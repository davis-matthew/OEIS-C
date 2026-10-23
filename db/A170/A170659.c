/*
 * A170659  Number of reduced words of length n in Coxeter group on 26
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies a(0) = 1 for 1 ≤ n ≤ 48 : a(n) =
 *            24·(a(n-1)+…+a(n-48)) + 2 for n = 49 : a(49) = 24·(a(48)+…+a(1))
 *            – 300·a(0) + 1 for n ≥ 50 : a(n) = 24·(a(n-1)+…+a(n-48)) –
 *            300·a(n-49)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Memoised recurrence implementation */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached terms and state */
    static seq_t *cache   = NULL;   /* array of computed a(i)   */
    static idx_t cache_sz = 0;      /* number of elements stored */
    static seq_t sum48    = 0;      /* sum of the last 48 stored terms */

    /* if already computed, return quickly */
    if (n < cache_sz)
        return cache[n];

    /* Ensure enough storage for all terms up to n */
    size_t needed = (size_t)n + 1;
    if (!cache)
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
    else if (needed > (size_t)cache_sz)
        cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!cache) abort();            /* allocation failure is fatal */

    /* initialise a(0) if nothing computed yet */
    if (cache_sz == 0) {
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        sum48    = cache[0];   /* sum of terms up to index 0 */
        cache_sz = 1;
    }

    /* compute terms from cache_sz up to n (inclusive) */
    for (idx_t i = cache_sz; i <= n; ++i) {
        seq_t new_val;
        if (i <= 48) {                     /* 1 … 48 */
            new_val = 24 * sum48 + 2;
        } else if (i == 49) {              /* 49 */
            new_val = 24 * sum48 - 300 * cache[0] + 1;
        } else {                           /* 50 … */
            new_val = 24 * sum48 - 300 * cache[i - 49];
        }

        cache[i] = new_val;

        /* update the sliding sum of the last 48 values */
        sum48 += new_val;
        if (i >= 48)
            sum48 -= cache[i - 48];

        cache_sz = i + 1;
    }

    return cache[n];
}
