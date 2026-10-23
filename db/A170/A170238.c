/*
 * A170238  Number of reduced words of length n in Coxeter group on 37
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A170238:
   a(0)=1.
   For n≥1:
     a(n) = 35*∑_{k=1..39} a(n-k)  – 630*a(n-40)  +  rhs_n,
   where rhs_n = 2 for 1≤n≤39, rhs_n = 1 for n=40, otherwise 0.
   (The term a(m) is taken as 0 for m<0.)
   The recurrence follows from the rational generating function
   (t^40 + 2t^39 + … + 2t + 1)/(630t^40 – 35t^39 – … – 35t + 1). */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;   /* pointer to allocated array of seq_t */
    static size_t cache_cap = 0;  /* current allocated capacity */
    static size_t cache_len = 0;  /* number of computed entries */

    /* Expand cache if necessary (doubling strategy) */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();           /* out of memory */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialise first element if cache empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        cache_len = 1;
    }

    /* Compute missing values up to n (inclusive) */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i == 0) continue;          /* already initialised */

        /* Sum of the previous 39 terms */
        seq_t sum = 0;
        for (idx_t k = 1; k <= 39 && i >= k; ++k)
            sum += cache[(size_t)(i - k)];

        /* 35 * sum using only shifts and additions */
        seq_t prod35 = (sum << 5) + (sum << 1) + sum;   /* 2^5 + 2 + 1 = 35 */

        /* 630 * a(i-40) if i ≥ 40, otherwise 0 */
        seq_t prod630 = 0;
        if (i >= 40) {
            seq_t x = cache[(size_t)(i - 40)];
            prod630 = (x << 9) + (x << 6) + (x << 3) + (x << 2) + (x << 1);
            /* 2^9 + 2^6 + 2^3 + 2^2 + 2^1 = 512 + 64 + 8 + 4 + 2 = 630 */
        }

        /* Right‑hand side constant term */
        seq_t rhs = 0;
        if (i <= 39)
            rhs = 2;
        else if (i == 40)
            rhs = 1;

        /* a(i) = prod35 - prod630 + rhs */
        cache[(size_t)i] = prod35 - prod630 + rhs;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
