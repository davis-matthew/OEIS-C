/*
 * A232983  The Gauss factorial n_7!.
 * https://oeis.org/A232983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{1≤j≤n, gcd(j,7)=1} j
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the Gauss factorial n_7! */
seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0; /* number of elements computed (<=cache_cap) */
    static size_t cache_cap = 0; /* allocated capacity */

    /* Ensure we can hold up to index n */
    if (n + 1 > cache_cap) {
        size_t new_cap = (n + 1) + 8;          /* simple growth factor */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);                    /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise base case a(0)=1 if not done yet */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Compute values up to requested n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        if ((i % 7ULL) != 0ULL) {
            cache[i] = prev * (seq_t)i;
        } else {
            cache[i] = prev;   /* skip multiples of 7 */
        }
    }
    cache_len = n + 1;
    return cache[n];
}
