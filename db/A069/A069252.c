/*
 * A069252  Number of basis partitions of n+81 with Durfee square size 9.
 * https://oeis.org/A069252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = N[n] – Σ_{k=1}^{31} D[k]·a(n-k) (with D[0] = 1)
 * Range:     u8 n <= 10, u16 n <= 29, u32 n <= 135, u64 n <= 2188,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 57 terms through n = 8503 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The coefficients D[0..31] and N[0..31] come from the rational
   generating function listed in the OEIS entry. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* binomial coefficients for (x-1)^9 */
static long long binom9(int k)
{
    long long r = 1;
    for (int i = 1; i <= k; ++i)
        r = r * (9 - i + 1) / i;
    return r;
}

/* multiply polynomial 'res' (degree *deg) by 'fac' (degree fac_deg) */
static void mult_poly(long long *res, int *deg,
                      const long long *fac, int fac_deg)
{
    long long tmp[64] = {0};
    for (int i = 0; i <= *deg; ++i) {
        if (res[i] == 0) continue;
        for (int j = 0; j <= fac_deg; ++j) {
            if (fac[j] == 0) continue;
            tmp[i + j] += res[i] * fac[j];
        }
    }
    *deg += fac_deg;
    for (int i = 0; i <= *deg; ++i)
        res[i] = tmp[i];
}

/* compute denominator coefficients D[0..31] (with D[0] = 1) and
   numerator coefficients N[0..31] (numerator of the generating function) */
static void init_polys(long long D[32], long long N[32])
{
    /* ----------- Denominator ----------- */
    long long res[64] = {0};
    res[0] = 1;
    int ddeg = 0;

    /* (x-1)^9 */
    long long f1[10];
    for (int j = 0; j <= 9; ++j) {
        long long c = binom9(j);
        if ((9 - j) & 1) c = -c;
        f1[j] = c;
    }
    mult_poly(res, &ddeg, f1, 9);

    /* (x^2 + x + 1)^3  */
    long long f2[3] = {1, 1, 1};
    for (int t = 0; t < 3; ++t)
        mult_poly(res, &ddeg, f2, 2);

    /* (x^4 + x^3 + x^2 + x + 1) */
    long long f3[5] = {1,1,1,1,1};
    mult_poly(res, &ddeg, f3, 4);

    /* (x^6 + x^3 + 1) */
    long long f4[7] = {1,0,0,1,0,0,1};
    mult_poly(res, &ddeg, f4, 6);

    /* (x^6 + x^5 + x^4 + x^3 + x^2 + x + 1) */
    long long f5[7] = {1,1,1,1,1,1,1};
    mult_poly(res, &ddeg, f5, 6);

    /* Record denominator coefficients, flipping sign so D[0]=1 */
    for (int i = 0; i <= 31; ++i)
        D[i] = -res[i];

    /* ----------- Numerator ---------- */
    long long resN[64] = {0};
    resN[0] = 1;
    int ndeg = 0;

    /* (x + 1) */
    long long g1[2] = {1,1};
    mult_poly(resN, &ndeg, g1, 1);

    /* (x^2 - x + 1) */
    long long g2[3] = {1,-1,1};
    mult_poly(resN, &ndeg, g2, 2);

    /* (x^4 - x^3 + x^2 - x + 1) */
    long long g3[5] = {1,-1,1,-1,1};
    mult_poly(resN, &ndeg, g3, 4);

    /* (x^4 - x^2 + 1) */
    long long g4[5] = {1,0,-1,0,1};
    mult_poly(resN, &ndeg, g4, 4);

    /* (x^6 - x^5 + x^4 - x^3 + x^2 - x + 1) */
    long long g5[7] = {1,-1,1,-1,1,-1,1};
    mult_poly(resN, &ndeg, g5, 6);

    /* (x^6 - x^3 + 1) */
    long long g6[7] = {1,0,0,-1,0,0,1};
    mult_poly(resN, &ndeg, g6, 6);

    /* (x^8 + 1) */
    long long g7[9] = {1,0,0,0,0,0,0,0,1};
    mult_poly(resN, &ndeg, g7, 8);

    /* Record numerator coefficients (no overall sign change) */
    for (int i = 0; i <= 31; ++i)
        N[i] = resN[i];
}

/* Sequence function */
seq_t a(idx_t n)
{
    /* static state */
    static int initialized = 0;
    static long long D[32];
    static long long N[32];
    if (!initialized) {
        init_polys(D, N);
        initialized = 1;
    }

    /* dynamic cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = mem_resize(seq_t, cache, new_cap);
        cache_cap = new_cap;
    }

    if (cache_len <= (size_t)n) {
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t sum = (i <= 31 ? (seq_t)N[i] : (seq_t)0);
            for (int k = 1; k <= 31; ++k) {
                if (i >= (size_t)k)
                    sum -= (seq_t)D[k] * cache[i - k];
            }
            cache[i] = sum;
        }
        cache_len = n + 1;
    }

    return cache[(size_t)n];
}
