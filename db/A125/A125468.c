/*
 * A125468  Number of base 11 circular n-digit numbers with adjacent digits
 *          differing by 9 or less.
 * https://oeis.org/A125468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 11*a(n-1) - a(n-2) - 9*a(n-3) for n > 3 a(0)=1, a(1)=11,
 *            a(2)=119, a(3)=1271
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes A125468: number of base‑11 circular n‑digit numbers
   with adjacent digits differing by 9 or less. */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static idx_t cache_len = 0; /* number of elements allocated (>=1) */
    static seq_t *cache = NULL; /* array of values a[0..cache_len-1] */

    /* Expand cache if needed to store a[n] */
    if ((idx_t)(n + 1) > cache_len) {
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!new_cache) abort();            /* allocation failure */
        cache = new_cache;

        /* Initialise the first four terms (they exist for all n >= 0) */
        if (n >= 0) cache[0] = (seq_t)1;
        if (n >= 1) cache[1] = (seq_t)11;
        if (n >= 2) cache[2] = (seq_t)119;
        if (n >= 3) cache[3] = (seq_t)1271;

        /* Compute remaining terms only if we don't already have them */
        idx_t start = (cache_len > 4) ? cache_len : 4;
        for (idx_t i = start; i <= n; ++i) {
            cache[i] = (seq_t)11 * cache[i-1]
                       - cache[i-2]
                       - (seq_t)9 * cache[i-3];
        }

        cache_len = n + 1;
    }

    return cache[n];
}
