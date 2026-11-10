/*
 * A236442  Number of ordered ways to write n = k + m with k > 0 and m > 0
 *          such that A000009(k) + A047967(m) is prime.
 * https://oeis.org/A236442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{k : 1 <= k <= n-1, A000009(k) + A047967(n-k) is prime},
 *            where A047967(m) = A000041(m) - A000009(m). The code finds the
 *            count by searching over k. A000041 and A000009 come from Euler's
 *            pentagonal recurrences and are held in fixed-width multi-limb
 *            integers, so they never overflow, whatever...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9963 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *A236442_P = NULL;   /* A000041 rows, L limbs each */
static uint32_t *A236442_Q = NULL;   /* A000009 rows, L limbs each */
static idx_t A236442_cap = 0;
static int A236442_L = 0;

static uint32_t A236442_sp[400];
static int A236442_nsp = 0;

/* sign of x^n in Product(1-x^(2m)): +1, -1, or 0 */
static int A236442_e2(idx_t n)
{
    idx_t m, j;
    if (n & 1) return 0;
    m = n / 2;
    for (j = 0;; j++) {
        idx_t g1 = (j == 0) ? 0 : j * (3 * j - 1) / 2;
        idx_t g2 = j * (3 * j + 1) / 2;
        if (g1 > m) break;
        if (g1 == m || g2 == m) return (j & 1) ? -1 : 1;
    }
    return 0;
}

static void A236442_add(uint32_t *d, const uint32_t *s, int L)
{
    uint64_t c = 0;
    int i;
    for (i = 0; i < L; i++) {
        c += (uint64_t)d[i] + s[i];
        d[i] = (uint32_t)c;
        c >>= 32;
    }
}

static void A236442_sub(uint32_t *d, const uint32_t *s, int L)
{
    uint64_t b = 0;
    int i;
    for (i = 0; i < L; i++) {
        uint64_t x = (uint64_t)d[i] - s[i] - b;
        d[i] = (uint32_t)x;
        b = (x >> 63) & 1;
    }
}

static void A236442_build(idx_t N)
{
    idx_t r = 0, i, k;
    int L, j;
    uint32_t *one;
    while ((r + 1) * (r + 1) <= N) r++;
    L = (int)((4 * r + 40) / 32 + 1);
    if (A236442_P) free(A236442_P);
    if (A236442_Q) free(A236442_Q);
    A236442_P = (uint32_t *)calloc((size_t)(N + 1) * L, sizeof(uint32_t));
    A236442_Q = (uint32_t *)calloc((size_t)(N + 1) * L, sizeof(uint32_t));
    one = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    one[0] = 1;
    for (i = 0; i <= N; i++) {
        uint32_t *p = A236442_P + (size_t)i * L;
        uint32_t *q = A236442_Q + (size_t)i * L;
        int e = A236442_e2(i);
        for (j = 0; j < L; j++) { p[j] = 0; q[j] = 0; }
        if (i == 0) { p[0] = 1; q[0] = 1; continue; }
        if (e > 0) A236442_add(q, one, L);
        else if (e < 0) A236442_sub(q, one, L);
        for (k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g1 > i) break;
            if (k & 1) {
                A236442_add(p, A236442_P + (size_t)(i - g1) * L, L);
                A236442_add(q, A236442_Q + (size_t)(i - g1) * L, L);
                if (g2 <= i) {
                    A236442_add(p, A236442_P + (size_t)(i - g2) * L, L);
                    A236442_add(q, A236442_Q + (size_t)(i - g2) * L, L);
                }
            } else {
                A236442_sub(p, A236442_P + (size_t)(i - g1) * L, L);
                A236442_sub(q, A236442_Q + (size_t)(i - g1) * L, L);
                if (g2 <= i) {
                    A236442_sub(p, A236442_P + (size_t)(i - g2) * L, L);
                    A236442_sub(q, A236442_Q + (size_t)(i - g2) * L, L);
                }
            }
        }
    }
    free(one);
    A236442_L = L;
    A236442_cap = N;
}

static void A236442_init_primes(void)
{
    int i, j;
    static char comp[2000];
    if (A236442_nsp) return;
    for (i = 2; i < 2000; i++) {
        if (comp[i]) continue;
        A236442_sp[A236442_nsp++] = (uint32_t)i;
        for (j = i * i; j < 2000; j += i) comp[j] = 1;
    }
}

static int A236442_cmp(const uint32_t *a, const uint32_t *b, int len)
{
    int i;
    for (i = len - 1; i >= 0; i--) {
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    return 0;
}

static void A236442_mont_mul(const uint32_t *a, const uint32_t *b,
                             const uint32_t *N, int len, uint32_t ninv,
                             uint32_t *res, uint32_t *t)
{
    int i, j;
    for (j = 0; j < len + 2; j++) t[j] = 0;
    for (i = 0; i < len; i++) {
        uint64_t C = 0, cs;
        uint32_t m;
        for (j = 0; j < len; j++) {
            cs = (uint64_t)t[j] + (uint64_t)a[j] * b[i] + C;
            t[j] = (uint32_t)cs;
            C = cs >> 32;
        }
        cs = (uint64_t)t[len] + C;
        t[len] = (uint32_t)cs;
        t[len + 1] = (uint32_t)(cs >> 32);
        m = t[0] * ninv;
        cs = (uint64_t)t[0] + (uint64_t)m * N[0];
        C = cs >> 32;
        for (j = 1; j < len; j++) {
            cs = (uint64_t)t[j] + (uint64_t)m * N[j] + C;
            t[j - 1] = (uint32_t)cs;
            C = cs >> 32;
        }
        cs = (uint64_t)t[len] + C;
        t[len - 1] = (uint32_t)cs;
        t[len] = t[len + 1] + (uint32_t)(cs >> 32);
    }
    if (t[len] != 0 || A236442_cmp(t, N, len) >= 0) A236442_sub(t, N, len);
    for (j = 0; j < len; j++) res[j] = t[j];
}

/* v = a * 2^(32*len) mod N, for small a < N */
static void A236442_to_mont(uint32_t a, const uint32_t *N, int len, uint32_t *v)
{
    int i, j;
    for (j = 0; j < len; j++) v[j] = 0;
    v[0] = a;
    for (i = 0; i < 32 * len; i++) {
        uint32_t carry = 0;
        for (j = 0; j < len; j++) {
            uint32_t nc = v[j] >> 31;
            v[j] = (v[j] << 1) | carry;
            carry = nc;
        }
        if (carry || A236442_cmp(v, N, len) >= 0) A236442_sub(v, N, len);
    }
}

/* s has len limbs (top nonzero); work buffers sized >= 6*(len+2) */
static bool A236442_is_prime(const uint32_t *s, int len, uint32_t *w)
{
    static const uint32_t bases[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    int i, j, b, r, top;
    uint32_t x0, ninv;
    uint32_t *oneM = w, *negM = w + (len + 2), *x = w + 2 * (len + 2);
    uint32_t *y = w + 3 * (len + 2), *t = w + 4 * (len + 2);
    if (len == 0) return false;
    if (len == 1 && s[0] < 2) return false;
    for (i = 0; i < A236442_nsp; i++) {
        uint32_t p = A236442_sp[i];
        uint64_t rem = 0;
        for (j = len - 1; j >= 0; j--) rem = ((rem << 32) | s[j]) % p;
        if (rem == 0) return (len == 1 && s[0] == p);
    }
    if (len == 1 && s[0] < 4000000u) return true;
    /* Montgomery setup */
    x0 = s[0];
    {
        uint32_t inv = x0;
        for (i = 0; i < 5; i++) inv *= 2u - x0 * inv;
        ninv = (uint32_t)(0u - inv);
    }
    /* s-1 = d * 2^r ; s odd */
    r = 0;
    {
        int lb = 0;
        uint32_t v = s[0] - 1;
        if (v == 0) {
            lb = 1;
            while (s[lb] == 0) lb++;
            v = s[lb];
            r = 32 * lb;
        }
        while (!(v & 1)) { v >>= 1; r++; }
    }
    top = 32 * len - 1;
    while (!((s[top / 32] >> (top % 32)) & 1)) top--;
    A236442_to_mont(1, s, len, oneM);
    for (j = 0; j < len; j++) negM[j] = s[j];
    A236442_sub(negM, oneM, len);
    for (i = 0; i < 12; i++) {
        bool ok = false;
        A236442_to_mont(bases[i], s, len, x);
        for (j = 0; j < len; j++) y[j] = oneM[j];
        for (b = top; b >= r; b--) {
            uint32_t bit;
            if (b == 0) bit = (s[0] - 1) & 1;
            else if (b / 32 == 0) bit = ((s[0] - 1) >> b) & 1;
            else bit = (s[b / 32] >> (b % 32)) & 1;
            A236442_mont_mul(y, y, s, len, ninv, y, t);
            if (bit) A236442_mont_mul(y, x, s, len, ninv, y, t);
        }
        if (A236442_cmp(y, oneM, len) == 0 || A236442_cmp(y, negM, len) == 0) ok = true;
        for (j = 1; j < r && !ok; j++) {
            A236442_mont_mul(y, y, s, len, ninv, y, t);
            if (A236442_cmp(y, negM, len) == 0) ok = true;
            else if (A236442_cmp(y, oneM, len) == 0) break;
        }
        if (!ok) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t k;
    seq_t cnt = 0;
    int L, len;
    uint32_t *s, *w;
    if (n < 2) return 0;
    A236442_init_primes();
    if (!A236442_P || n - 1 > A236442_cap) {
        idx_t c = A236442_cap ? A236442_cap : 64;
        while (c < n) c *= 2;
        A236442_build(c);
    }
    L = A236442_L;
    s = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)L);
    w = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)(6 * (L + 2)));
    for (k = 1; k < n; k++) {
        idx_t m = n - k;
        int j;
        const uint32_t *qk = A236442_Q + (size_t)k * L;
        for (j = 0; j < L; j++) s[j] = qk[j];
        A236442_add(s, A236442_P + (size_t)m * L, L);
        A236442_sub(s, A236442_Q + (size_t)m * L, L);
        len = L;
        while (len > 0 && s[len - 1] == 0) len--;
        if (len == 0) continue;
        if (!(s[0] & 1) && !(len == 1 && s[0] == 2)) continue;
        if (A236442_is_prime(s, len, w)) cnt++;
    }
    free(s);
    free(w);
    return cnt;
}
