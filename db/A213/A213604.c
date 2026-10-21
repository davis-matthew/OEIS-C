/*
 * A213604  Cumulative sums of digital roots of A005891(n).
 * https://oeis.org/A213604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(n+9) = -a(n) + a(n+1) + a(n+8)
   Initial values:  a(0)=1, a(1)=7, a(2)=14, a(3)=18, a(4)=24,
                    a(5)=28, a(6)=35, a(7)=41, a(8)=42 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for already computed terms. */
    static seq_t *cache = NULL;          /* array of computed terms   */
    static idx_t cache_len = 0;           /* number of entries in cache */

    /* Initialise cache with the first nine terms when first called. */
    if (cache_len == 0) {
        cache_len = 9;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failure – return 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)14;
        cache[3] = (seq_t)18;
        cache[4] = (seq_t)24;
        cache[5] = (seq_t)28;
        cache[6] = (seq_t)35;
        cache[7] = (seq_t)41;
        cache[8] = (seq_t)42;
    }

    /* If the requested index is already inside the cache, return it. */
    if (n < cache_len) {
        return cache[n];
    }

    /* Extend the cache to include the requested index. */
    idx_t new_len = n + 1;                 /* new number of terms needed   */
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failed – keep old cache and return 0 */
        return (seq_t)0;
    }
    cache = new_cache;

    /* Compute all missing terms using the recurrence. */
    for (idx_t i = cache_len; i < new_len; ++i) {
        /* a(i) = a(i-1) + a(i-8) - a(i-9) 
           (rearranged from a(i+9) = -a(i) + a(i+1) + a(i+8)) */
        seq_t sum = cache[i - 1] + cache[i - 8];
        seq_t term = sum - cache[i - 9];
        cache[i] = term;
    }

    cache_len = new_len;
    return cache[n];
}
