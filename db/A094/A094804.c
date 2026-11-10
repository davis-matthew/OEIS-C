/*
 * A094804  Number of primes in the trajectory of n under the juggler map of
 *          A094683.
 * https://oeis.org/A094804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   iterate the juggler map of A094683, v -> floor(sqrt(v)) for even
 *            v, v -> floor(sqrt(v^3)) for odd v, starting at v = n until v <=
 *            1, and count the primes met along the way (multi-precision
 *            arithmetic is used internally, since trajectories exceed 64
 *            bits)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 105 terms through n = 104 (2026-11-07)
 * Helpers:   is_prime_u64, isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/isqrt_u64.h"

typedef struct { uint32_t *d; int n; } A094804_bn;

static int A094804_trim(const uint32_t *d, int n)
{
    while (n > 0 && d[n - 1] == 0) n--;
    return n;
}

static A094804_bn A094804_alloc(int n)
{
    A094804_bn r;
    r.d = (uint32_t *)calloc((size_t)(n > 0 ? n : 1), sizeof(uint32_t));
    r.n = n;
    return r;
}

static void A094804_free(A094804_bn *a) { free(a->d); a->d = NULL; a->n = 0; }

static A094804_bn A094804_from_u64(uint64_t v)
{
    A094804_bn r = A094804_alloc(2);
    r.d[0] = (uint32_t)v;
    r.d[1] = (uint32_t)(v >> 32);
    r.n = A094804_trim(r.d, 2);
    return r;
}

static uint64_t A094804_to_u64(const A094804_bn *x)
{
    uint64_t v = 0;
    if (x->n >= 1) v = x->d[0];
    if (x->n >= 2) v |= (uint64_t)x->d[1] << 32;
    return v;
}

static int A094804_cmp(const A094804_bn *a, const A094804_bn *b)
{
    int i;
    if (a->n != b->n) return a->n < b->n ? -1 : 1;
    for (i = a->n - 1; i >= 0; i--)
        if (a->d[i] != b->d[i]) return a->d[i] < b->d[i] ? -1 : 1;
    return 0;
}

static int A094804_bitlen32(uint32_t x)
{
    int b = 0;
    while (x) { b++; x >>= 1; }
    return b;
}

static int A094804_nlz32(uint32_t x) { return 32 - A094804_bitlen32(x); }

static A094804_bn A094804_mul(const A094804_bn *a, const A094804_bn *b)
{
    A094804_bn r = A094804_alloc(a->n + b->n);
    int i, j;
    for (i = 0; i < a->n; i++) {
        uint64_t c = 0, ai = a->d[i];
        if (!ai) continue;
        for (j = 0; j < b->n; j++) {
            uint64_t t = ai * b->d[j] + r.d[i + j] + c;
            r.d[i + j] = (uint32_t)t;
            c = t >> 32;
        }
        r.d[i + b->n] = (uint32_t)c;
    }
    r.n = A094804_trim(r.d, r.n);
    return r;
}

static A094804_bn A094804_add(const A094804_bn *a, const A094804_bn *b)
{
    int mx = a->n > b->n ? a->n : b->n, i;
    A094804_bn r = A094804_alloc(mx + 1);
    uint64_t c = 0;
    for (i = 0; i < mx; i++) {
        uint64_t t = c;
        if (i < a->n) t += a->d[i];
        if (i < b->n) t += b->d[i];
        r.d[i] = (uint32_t)t;
        c = t >> 32;
    }
    r.d[mx] = (uint32_t)c;
    r.n = A094804_trim(r.d, mx + 1);
    return r;
}

static void A094804_shr1(A094804_bn *a)
{
    int i;
    for (i = 0; i < a->n; i++) {
        uint32_t hi = (i + 1 < a->n) ? a->d[i + 1] : 0;
        a->d[i] = (a->d[i] >> 1) | (hi << 31);
    }
    a->n = A094804_trim(a->d, a->n);
}

/* quotient floor(u / v), v != 0 (Knuth algorithm D, 32-bit digits) */
static A094804_bn A094804_div(const A094804_bn *U, const A094804_bn *V)
{
    int m = U->n, n = V->n, i, j;
    A094804_bn q;
    if (A094804_cmp(U, V) < 0) return A094804_alloc(0);
    q = A094804_alloc(m - n + 1);
    if (n == 1) {
        uint64_t k = 0, dv = V->d[0];
        for (j = m - 1; j >= 0; j--) {
            uint64_t cur = (k << 32) | U->d[j];
            q.d[j] = (uint32_t)(cur / dv);
            k = cur - (uint64_t)q.d[j] * dv;
        }
        q.n = A094804_trim(q.d, m);
        return q;
    }
    {
        const uint64_t b = 4294967296ULL;
        int s = A094804_nlz32(V->d[n - 1]);
        uint32_t *vn = (uint32_t *)calloc((size_t)n, sizeof(uint32_t));
        uint32_t *un = (uint32_t *)calloc((size_t)(m + 1), sizeof(uint32_t));
        const uint32_t *u = U->d, *v = V->d;
        for (i = n - 1; i > 0; i--)
            vn[i] = (uint32_t)(((uint64_t)v[i] << s) | ((uint64_t)v[i - 1] >> (32 - s)));
        vn[0] = (uint32_t)((uint64_t)v[0] << s);
        un[m] = (uint32_t)((uint64_t)u[m - 1] >> (32 - s));
        for (i = m - 1; i > 0; i--)
            un[i] = (uint32_t)(((uint64_t)u[i] << s) | ((uint64_t)u[i - 1] >> (32 - s)));
        un[0] = (uint32_t)((uint64_t)u[0] << s);

        for (j = m - n; j >= 0; j--) {
            uint64_t num = ((uint64_t)un[j + n] << 32) | un[j + n - 1];
            uint64_t qhat = num / vn[n - 1];
            uint64_t rhat = num - qhat * vn[n - 1];
            int64_t t, k;
            while (qhat >= b ||
                   qhat * vn[n - 2] > ((rhat << 32) | un[j + n - 2])) {
                qhat--;
                rhat += vn[n - 1];
                if (rhat >= b) break;
            }
            k = 0;
            for (i = 0; i < n; i++) {
                uint64_t p = qhat * vn[i];
                t = (int64_t)un[i + j] - k - (int64_t)(p & 0xFFFFFFFFULL);
                un[i + j] = (uint32_t)t;
                k = (int64_t)(p >> 32) - (t >> 32);
            }
            t = (int64_t)un[j + n] - k;
            un[j + n] = (uint32_t)t;
            q.d[j] = (uint32_t)qhat;
            if (t < 0) {
                uint64_t c = 0;
                q.d[j]--;
                for (i = 0; i < n; i++) {
                    uint64_t s2 = (uint64_t)un[i + j] + vn[i] + c;
                    un[i + j] = (uint32_t)s2;
                    c = s2 >> 32;
                }
                un[j + n] = (uint32_t)(un[j + n] + c);
            }
        }
        free(vn);
        free(un);
    }
    q.n = A094804_trim(q.d, m - n + 1);
    return q;
}

static A094804_bn A094804_isqrt(const A094804_bn *x)
{
    int bits, e;
    A094804_bn y;
    if (x->n <= 2)
        return A094804_from_u64(isqrt_u64(A094804_to_u64(x)));
    bits = 32 * (x->n - 1) + A094804_bitlen32(x->d[x->n - 1]);
    e = (bits + 1) / 2;                       /* 2^e >= sqrt(x) */
    y = A094804_alloc(e / 32 + 1);
    y.d[e / 32] = 1u << (e % 32);
    for (;;) {
        A094804_bn q = A094804_div(x, &y);
        A094804_bn z = A094804_add(&y, &q);
        A094804_free(&q);
        A094804_shr1(&z);
        if (A094804_cmp(&z, &y) >= 0) { A094804_free(&z); break; }
        A094804_free(&y);
        y = z;
    }
    return y;
}

/* ---- Montgomery arithmetic for Miller-Rabin on multi-limb odd N ---- */
static bool A094804_geq(const uint32_t *x, const uint32_t *N, int L)
{
    int i;
    for (i = L - 1; i >= 0; i--)
        if (x[i] != N[i]) return x[i] > N[i];
    return true;
}

static void A094804_subN(uint32_t *x, const uint32_t *N, int L)
{
    int64_t br = 0;
    int i;
    for (i = 0; i < L; i++) {
        int64_t t = (int64_t)x[i] - N[i] - br;
        br = t < 0;
        x[i] = (uint32_t)t;
    }
}

static void A094804_moddbl(uint32_t *x, const uint32_t *N, int L)
{
    uint32_t carry = x[L - 1] >> 31;
    int i;
    for (i = L - 1; i > 0; i--) x[i] = (x[i] << 1) | (x[i - 1] >> 31);
    x[0] <<= 1;
    if (carry || A094804_geq(x, N, L)) A094804_subN(x, N, L);
}

static void A094804_modadd(uint32_t *x, const uint32_t *y, const uint32_t *N, int L)
{
    uint64_t c = 0;
    int i;
    for (i = 0; i < L; i++) {
        uint64_t t = (uint64_t)x[i] + y[i] + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
    if (c || A094804_geq(x, N, L)) A094804_subN(x, N, L);
}

static void A094804_montmul(const uint32_t *a, const uint32_t *b, uint32_t *out,
                            const uint32_t *N, int L, uint32_t ninv, uint32_t *t)
{
    int i, j;
    memset(t, 0, sizeof(uint32_t) * (size_t)(L + 2));
    for (i = 0; i < L; i++) {
        uint64_t C = 0, cs, bi = b[i];
        uint32_t m;
        for (j = 0; j < L; j++) {
            cs = (uint64_t)t[j] + (uint64_t)a[j] * bi + C;
            t[j] = (uint32_t)cs;
            C = cs >> 32;
        }
        cs = (uint64_t)t[L] + C;
        t[L] = (uint32_t)cs;
        t[L + 1] = (uint32_t)(cs >> 32);
        m = t[0] * ninv;
        cs = (uint64_t)t[0] + (uint64_t)m * N[0];
        C = cs >> 32;
        for (j = 1; j < L; j++) {
            cs = (uint64_t)t[j] + (uint64_t)m * N[j] + C;
            t[j - 1] = (uint32_t)cs;
            C = cs >> 32;
        }
        cs = (uint64_t)t[L] + C;
        t[L - 1] = (uint32_t)cs;
        t[L] = t[L + 1] + (uint32_t)(cs >> 32);
    }
    if (t[L] || A094804_geq(t, N, L)) A094804_subN(t, N, L);
    memcpy(out, t, sizeof(uint32_t) * (size_t)L);
}

static bool A094804_mr(const A094804_bn *Nb, int base)
{
    int L = Nb->n, k, i, s, top;
    const uint32_t *N = Nb->d;
    uint32_t inv = N[0], ninv;
    uint32_t *one, *mone, *x, *tmp, *t, *nm1;
    bool res = false;

    for (k = 0; k < 5; k++) inv *= 2u - N[0] * inv;
    ninv = (uint32_t)0 - inv;

    one  = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    mone = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    x    = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    tmp  = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    nm1  = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    t    = (uint32_t *)calloc((size_t)(L + 2), sizeof(uint32_t));

    one[0] = 1;
    for (k = 0; k < 32 * L; k++) A094804_moddbl(one, N, L);   /* R mod N */
    memcpy(mone, N, sizeof(uint32_t) * (size_t)L);
    {
        int64_t br = 0;
        for (i = 0; i < L; i++) {
            int64_t tt = (int64_t)mone[i] - one[i] - br;
            br = tt < 0;
            mone[i] = (uint32_t)tt;
        }
    }
    memcpy(nm1, N, sizeof(uint32_t) * (size_t)L);
    nm1[0] -= 1;                                   /* N odd: no borrow */
    s = 0;
    while (!((nm1[s / 32] >> (s % 32)) & 1u)) s++;
    top = 32 * (L - 1) + A094804_bitlen32(nm1[L - 1]) - 1;

    memcpy(x, one, sizeof(uint32_t) * (size_t)L);
    for (i = top; i >= s; i--) {
        A094804_montmul(x, x, x, N, L, ninv, t);
        if ((nm1[i / 32] >> (i % 32)) & 1u) {
            if (base == 2) {
                A094804_moddbl(x, N, L);
            } else {
                memcpy(tmp, x, sizeof(uint32_t) * (size_t)L);
                A094804_moddbl(x, N, L);
                A094804_modadd(x, tmp, N, L);
            }
        }
    }
    if (memcmp(x, one, sizeof(uint32_t) * (size_t)L) == 0 ||
        memcmp(x, mone, sizeof(uint32_t) * (size_t)L) == 0) {
        res = true;
    } else {
        for (k = 1; k < s; k++) {
            A094804_montmul(x, x, x, N, L, ninv, t);
            if (memcmp(x, mone, sizeof(uint32_t) * (size_t)L) == 0) { res = true; break; }
            if (memcmp(x, one, sizeof(uint32_t) * (size_t)L) == 0) break;
        }
    }
    free(one); free(mone); free(x); free(tmp); free(nm1); free(t);
    return res;
}

static bool A094804_isprime(const A094804_bn *x)
{
    static uint32_t small[400];
    static int nsmall = 0;
    int i, j;
    if (x->n <= 2) return is_prime_u64(A094804_to_u64(x));
    if (!(x->d[0] & 1u)) return false;
    if (nsmall == 0) {
        uint32_t p;
        for (p = 3; p < 2000; p += 2) {
            uint32_t q;
            bool pr = true;
            for (q = 3; q * q <= p; q += 2)
                if (p % q == 0) { pr = false; break; }
            if (pr) small[nsmall++] = p;
        }
    }
    for (j = 0; j < nsmall; j++) {
        uint64_t r = 0, p = small[j];
        for (i = x->n - 1; i >= 0; i--) r = ((r << 32) | x->d[i]) % p;
        if (r == 0) return false;
    }
    return A094804_mr(x, 2) && A094804_mr(x, 3);
}

extern seq_t a(idx_t n)
{
    A094804_bn v = A094804_from_u64((uint64_t)n);
    seq_t cnt = 0;
    while (!(v.n == 0 || (v.n == 1 && v.d[0] == 1))) {
        A094804_bn w;
        if (A094804_isprime(&v)) cnt++;
        if (!(v.d[0] & 1u)) {
            w = A094804_isqrt(&v);
        } else {
            A094804_bn sq = A094804_mul(&v, &v);
            A094804_bn cu = A094804_mul(&sq, &v);
            A094804_free(&sq);
            w = A094804_isqrt(&cu);
            A094804_free(&cu);
        }
        A094804_free(&v);
        v = w;
    }
    A094804_free(&v);
    return cnt;
}
