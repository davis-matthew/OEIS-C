/*
 * A000146  From von Staudt-Clausen representation of Bernoulli numbers: a(n)
 *          = Bernoulli(2n) + Sum_{(p-1)|2n} 1/p.
 * https://oeis.org/A000146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, nice, easy
 * Formula:   a(n) = Bernoulli(2n) + Sum_{p prime, (p-1)|2n} 1/p, where
 *            Bernoulli(m) = Sum_{k=1..m} (-1)^k * binomial(m+1,k+1) *
 *            (1^m+2^m+...+k^m)/(k+1). a(n) is evaluated modulo enough 62-bit
 *            primes and recombined exactly by CRT (Garner)
 * Range:     i8 n <= 9, i16 n <= 11, i32 n <= 15, i64 n <= 21, i128 n <= 31,
 *            big any n
 * Helpers:   a146_bitlen
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a146_bitlen.h"

typedef unsigned __int128 a146_u128;

static uint64_t a146_mulmod(uint64_t x, uint64_t y, uint64_t p)
{
    return (uint64_t)(((a146_u128)x * y) % p);
}

static uint64_t a146_powmod(uint64_t x, uint64_t e, uint64_t p)
{
    uint64_t r = 1;
    x %= p;
    while (e) {
        if (e & 1) r = a146_mulmod(r, x, p);
        x = a146_mulmod(x, x, p);
        e >>= 1;
    }
    return r;
}

static bool a146_isprime(uint64_t n)
{
    static const uint64_t sp[25] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};
    static const uint64_t bases[12] = {2,3,5,7,11,13,17,19,23,29,31,37};
    uint64_t d;
    int s = 0;
    size_t i;
    if (n < 2) return false;
    for (i = 0; i < 25; i++)
        if (n % sp[i] == 0) return n == sp[i];
    d = n - 1;
    while (!(d & 1)) { d >>= 1; s++; }
    for (i = 0; i < 12; i++) {
        uint64_t x = a146_powmod(bases[i], d, n);
        int r;
        if (x == 1 || x == n - 1) continue;
        for (r = 1; r < s; r++) {
            x = a146_mulmod(x, x, n);
            if (x == n - 1) break;
        }
        if (r == s) return false;
    }
    return true;
}

/* cache of primes just below 2^62, descending */
static uint64_t *a146_pr = NULL;
static size_t a146_npr = 0, a146_cap = 0;

static void a146_need(size_t k)
{
    uint64_t c = a146_npr ? a146_pr[a146_npr - 1] - 2 : ((uint64_t)1 << 62) - 1;
    while (a146_npr < k) {
        if (a146_isprime(c)) {
            if (a146_npr == a146_cap) {
                a146_cap = a146_cap ? 2 * a146_cap : 64;
                a146_pr = (uint64_t *)realloc(a146_pr, a146_cap * sizeof(uint64_t));
            }
            a146_pr[a146_npr++] = c;
        }
        c -= 2;
    }
}

/* Montgomery arithmetic for odd p < 2^62 */
typedef struct { uint64_t p, np, r1, r2; } a146_mont;

static void a146_minit(a146_mont *M, uint64_t p)
{
    uint64_t inv = p;
    int i;
    for (i = 0; i < 5; i++) inv *= 2 - p * inv;
    M->p = p;
    M->np = (uint64_t)0 - inv;
    M->r1 = (uint64_t)((((a146_u128)1) << 64) % p);
    M->r2 = (uint64_t)(((a146_u128)M->r1 * M->r1) % p);
}

static inline uint64_t a146_redc(const a146_mont *M, a146_u128 T)
{
    uint64_t q = (uint64_t)T * M->np;
    uint64_t r = (uint64_t)((T + (a146_u128)q * M->p) >> 64);
    return r >= M->p ? r - M->p : r;
}

static inline uint64_t a146_mul(const a146_mont *M, uint64_t x, uint64_t y)
{
    return a146_redc(M, (a146_u128)x * y);
}

static inline uint64_t a146_add(const a146_mont *M, uint64_t x, uint64_t y)
{
    uint64_t s = x + y;
    return s >= M->p ? s - M->p : s;
}

static inline uint64_t a146_sub(const a146_mont *M, uint64_t x, uint64_t y)
{
    return x >= y ? x - y : x + M->p - y;
}

static uint64_t a146_mpow(const a146_mont *M, uint64_t b, uint64_t e)
{
    uint64_t r = M->r1;
    while (e) {
        if (e & 1) r = a146_mul(M, r, b);
        b = a146_mul(M, b, b);
        e >>= 1;
    }
    return r;
}

/* a(n) mod p, with m = 2n */
static uint64_t a146_res(uint64_t p, uint64_t m, const uint32_t *spf,
                         const uint64_t *pl, size_t np, const uint64_t *qs, size_t nq,
                         uint64_t *mt, uint64_t *pw, uint64_t *iv)
{
    a146_mont M;
    uint64_t i, k, t, S = 0, C, B = 0;
    size_t s;
    int b, top = 63;
    a146_minit(&M, p);
    mt[0] = 0;
    for (i = 1; i <= m + 1; i++) mt[i] = a146_add(&M, mt[i - 1], M.r1);
    /* j^m for primes j <= m, all exponentiations interleaved */
    while (!((m >> top) & 1)) top--;
    for (s = 0; s < np; s++) pw[pl[s]] = mt[pl[s]];
    for (b = top - 1; b >= 0; b--) {
        int bit = (int)((m >> b) & 1);
        for (s = 0; s < np; s++) {
            uint64_t x = pw[pl[s]];
            x = a146_mul(&M, x, x);
            if (bit) x = a146_mul(&M, x, mt[pl[s]]);
            pw[pl[s]] = x;
        }
    }
    pw[1] = M.r1;
    for (i = 2; i <= m; i++)
        if (spf[i] != i) pw[i] = a146_mul(&M, pw[spf[i]], pw[i / spf[i]]);
    /* inverses of 1..m+1 by batch inversion */
    iv[0] = M.r1;
    for (i = 1; i <= m + 1; i++) iv[i] = a146_mul(&M, iv[i - 1], mt[i]);
    t = a146_mpow(&M, iv[m + 1], p - 2);
    for (i = m + 1; i >= 1; i--) {
        iv[i] = a146_mul(&M, t, iv[i - 1]);
        t = a146_mul(&M, t, mt[i]);
    }
    C = mt[m + 1];                      /* binomial(m+1, k+1) */
    for (k = 1; k <= m; k++) {
        uint64_t tm;
        S = a146_add(&M, S, pw[k]);
        C = a146_mul(&M, C, a146_mul(&M, mt[m + 1 - k], iv[k + 1]));
        tm = a146_mul(&M, a146_mul(&M, C, iv[k + 1]), S);
        B = (k & 1) ? a146_sub(&M, B, tm) : a146_add(&M, B, tm);
    }
    for (s = 0; s < nq; s++) B = a146_add(&M, B, iv[qs[s]]);
    return a146_redc(&M, B);
}

seq_t a(idx_t n)
{
    uint64_t m = 2 * (uint64_t)n, L = 0, acc = 1, T, k, i, j;
    size_t K, nq = 0, np = 0, s, r;
    uint32_t *spf;
    uint64_t *qs, *pl, *mt, *pw, *iv, *res;
    int64_t *dg;
    seq_t v = 0;

    /* bound: log2|B(2n)| < 2 + log2((2n)!) - 5n, L >= log2((2n)!) */
    for (k = 2; k <= m; k++) {
        if (acc > UINT64_MAX / k) { L += a146_bitlen(acc); acc = 1; }
        acc *= k;
    }
    L += a146_bitlen(acc);
    T = (L + 2 > 5 * (uint64_t)n ? L + 2 - 5 * (uint64_t)n : 0) + 72;
    K = (size_t)(T / 61 + 1);
    a146_need(K);

    spf = (uint32_t *)calloc((size_t)(m + 2), sizeof(uint32_t));
    for (i = 2; i <= m + 1; i++)
        if (!spf[i]) {
            spf[i] = (uint32_t)i;
            for (j = i * i; j <= m + 1; j += i)
                if (!spf[j]) spf[j] = (uint32_t)i;
        }
    qs = (uint64_t *)malloc((size_t)(m + 2) * sizeof(uint64_t));
    pl = (uint64_t *)malloc((size_t)(m + 2) * sizeof(uint64_t));
    for (i = 1; i <= m; i++)
        if (m % i == 0 && spf[i + 1] == i + 1) qs[nq++] = i + 1;
    for (i = 2; i <= m; i++)
        if (spf[i] == i) pl[np++] = i;

    mt = (uint64_t *)malloc((size_t)(m + 2) * sizeof(uint64_t));
    pw = (uint64_t *)malloc((size_t)(m + 2) * sizeof(uint64_t));
    iv = (uint64_t *)malloc((size_t)(m + 2) * sizeof(uint64_t));
    res = (uint64_t *)malloc(K * sizeof(uint64_t));
    dg = (int64_t *)malloc(K * sizeof(int64_t));

    for (s = 0; s < K; s++)
        res[s] = a146_res(a146_pr[s], m, spf, pl, np, qs, nq, mt, pw, iv);

    /* Garner with balanced mixed-radix digits */
    for (s = 0; s < K; s++) {
        a146_mont M;
        uint64_t p = a146_pr[s], x = 0, pr, c;
        a146_minit(&M, p);
        pr = M.r1;
        for (r = s; r-- > 0;) {
            uint64_t pj = a146_pr[r] >= p ? a146_pr[r] - p : a146_pr[r];
            uint64_t cj = dg[r] >= 0 ? (uint64_t)dg[r] : p - (uint64_t)(-dg[r]);
            pj = a146_mul(&M, pj, M.r2);
            x = a146_add(&M, a146_mul(&M, x, pj), a146_mul(&M, cj, M.r2));
            pr = a146_mul(&M, pr, pj);
        }
        c = a146_mul(&M, a146_sub(&M, a146_mul(&M, res[s], M.r2), x),
                     a146_mpow(&M, pr, p - 2));
        c = a146_redc(&M, c);
        dg[s] = c > p / 2 ? (int64_t)c - (int64_t)p : (int64_t)c;
    }

    for (s = K; s-- > 0;) {
        v *= (idx_t)a146_pr[s];
        if (dg[s] >= 0) v += (idx_t)dg[s];
        else v -= (idx_t)(-dg[s]);
    }

    free(spf); free(qs); free(pl); free(mt); free(pw); free(iv); free(res); free(dg);
    return v;
}
