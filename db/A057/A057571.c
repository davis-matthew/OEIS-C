/*
 * A057571  Area under Dyck paths.
 * https://oeis.org/A057571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = 2 * (sum of areas under all Dyck paths of length n)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 26, u64 n <= 57, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (n >= 4):
 * (n+1)*(4*n^3 - 28*n^2 + 55*n - 27)*a(n) =
 *   2*(8*n^3 - 48*n^2 + 52*n + 27)*a(n-1)
 * + 4*(2*n - 1)*(4*n^3 - 24*n^2 + 29*n + 18)*a(n-2)
 * - 16*(2*n - 3)*(2*n^2 - 8*n - 1)*a(n-3)
 * - 16*(n-3)*(4*n^3 - 16*n^2 + 11*n + 4)*a(n-4)   */

#include <stdlib.h>

enum { SEQ_T_SIZE = sizeof(seq_t) };

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t  cache_cap = 0;
    static idx_t  cache_max = 0;   /* last index for which a() is known */

    /* ---------- first time: initialise small cache and base values ---------- */
    if (!cache)
    {
        cache_cap = 8;                     /* sufficient for the first calls   */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)19;
        cache_max = 3;
    }

    /* ---------- grow the cache if the requested index is outside ---------- */
    if (n >= cache_cap)
    {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* ---------- already computed? ---------- */
    if (n <= cache_max) return cache[n];

    /* ---------- compute missing values ---------- */
    for (idx_t i = cache_max + 1; i <= n; ++i)
    {
        if (i < 4)           /* base case – already stored */
            continue;

        seq_t result;
#ifdef __SIZEOF_INT128__
        /* ---------- 64‑bit seq_t: use 128‑bit intermediate ---------- */
        if (SEQ_T_SIZE <= 8)
        {
            typedef unsigned __int128 u128;
            u128 ii  = (u128)i;
            u128 c1  = 8*ii*ii*ii - 48*ii*ii + 52*ii + 27;
            u128 c2  = 2*ii - 1;
            u128 c3  = 4*ii*ii*ii - 24*ii*ii + 29*ii + 18;
            u128 c4  = 2*ii - 3;
            u128 c5  = 2*ii*ii - 8*ii - 1;
            u128 c6  = ii - 3;
            u128 c7  = 4*ii*ii*ii - 16*ii*ii + 11*ii + 4;
            u128 denom_coef = 4*ii*ii*ii - 28*ii*ii + 55*ii - 27;
            u128 denom = (ii + 1) * denom_coef;

            u128 term1 = 2 * c1 * (u128)cache[i-1];
            u128 term2 = 4 * c2 * c3 * (u128)cache[i-2];
            u128 term3 = 16 * c4 * c5 * (u128)cache[i-3];
            u128 term4 = 16 * c6 * c7 * (u128)cache[i-4];

            u128 sum34 = term3 + term4;
            u128 numer = term1 + term2;
            numer -= sum34;

            result = (seq_t)(numer / denom);
        }
        else
#endif
        {
            /* ---------- arbitrary‑precision seq_t ---------- */
            seq_t ii   = (seq_t)i;
            seq_t c1   = 8*ii*ii*ii - 48*ii*ii + 52*ii + 27;
            seq_t c2   = 2*ii - 1;
            seq_t c3   = 4*ii*ii*ii - 24*ii*ii + 29*ii + 18;
            seq_t c4   = 2*ii - 3;
            seq_t c5   = 2*ii*ii - 8*ii - 1;
            seq_t c6   = ii - 3;
            seq_t c7   = 4*ii*ii*ii - 16*ii*ii + 11*ii + 4;
            seq_t denom_coef = 4*ii*ii*ii - 28*ii*ii + 55*ii - 27;
            seq_t denom = (ii + 1) * denom_coef;

            seq_t term1 = 2 * c1 * cache[i-1];
            seq_t term2 = 4 * c2 * c3 * cache[i-2];
            seq_t term3 = 16 * c4 * c5 * cache[i-3];
            seq_t term4 = 16 * c6 * c7 * cache[i-4];

            seq_t sum34 = term3 + term4;
            seq_t numer = term1 + term2;
            numer -= sum34;

            result = numer / denom;
        }

        cache[i] = result;
    }

    cache_max = n;
    return cache[n];
}
