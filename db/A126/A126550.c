/*
 * A126550  Number of base 29 n-digit numbers with adjacent digits differing
 *          by five or less.
 * https://oeis.org/A126550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A126550: Number of base 29 n‑digit numbers with adjacent digits differing by five or less. */
/* Computation uses simple dynamic programming: for each length L we keep the count of
   strings of length L that end with a particular digit.  The next length is obtained
   by summing the previous counts of all digits at distance ≤5.  The total for length
   L is the sum of this array.  Results are cached so that subsequent calls reuse
   already computed values. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached values: cache[i] = a(i) */
    static seq_t *cache = NULL;
    static idx_t  cache_size   = 0; /* number of values stored (max index = cache_size - 1) */
    static idx_t  cache_cap    = 0; /* allocated capacity */

    /* Current DP array for the last computed length */
    static seq_t cur[29];

    /* Initialise cache and first DP state on first call */
    if (cache_size == 0) {
        cache_cap = 2;                    /* at least space for a(0) and a(1) */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1;                     /* a(0) */
        cache[1] = 29;                    /* a(1) */
        cache_size = 2;                   /* indices 0..1 stored */

        /* cur represents length 1: one string ending in every digit */
        for (int d = 0; d < 29; ++d) cur[d] = 1;
    }

    /* Return cached value if already computed */
    if (n < cache_size) return cache[n];

    /* Grow cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 10;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute successive lengths from cache_size up to n */
    for (idx_t len = cache_size; len <= n; ++len) {
        seq_t next[29];
        for (int d = 0; d < 29; ++d) {
            seq_t sum = 0;
            int start = d - 5;
            if (start < 0) start = 0;
            int end = d + 5;
            if (end > 28) end = 28;
            for (int k = start; k <= end; ++k) {
                sum += cur[k];
            }
            next[d] = sum;
        }

        /* Total for this length */
        seq_t total = 0;
        for (int d = 0; d < 29; ++d) total += next[d];

        /* Store the result */
        cache[len] = total;

        /* Prepare cur for next iteration */
        for (int d = 0; d < 29; ++d) cur[d] = next[d];
    }

    cache_size = n + 1;
    return cache[n];
}
