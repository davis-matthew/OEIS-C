/*
 * A140368  Composites of the form 26k + 17.
 * https://oeis.org/A140368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) is the nth composite number of the form 26k + 17
 * Range:     u8 n <= 5, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

/* Return the nth composite number congruent to 17 modulo 26.
   The offset is 1: a(1) = 69, a(2) = 95, ...                 */
seq_t a(idx_t n) {
    /* Handle the trivial case */
    if (n == 0) {
        return 0;
    }

    /* Static cache to allow O(1) answer for repeated calls */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of elements in cache   */
    static size_t cache_cap  = 0;   /* allocated capacity of cache   */
    static idx_t  next_k     = 0;   /* next k to test                */

    /* Grow the cache until we have at least n elements   */
    while (cache_size < n) {
        /* Compute current candidate: 26*k + 17   */
        seq_t candidate = (seq_t)26 * next_k + 17;

        /* Skip 1 (not needed) and keep only composites  */
        if (candidate > 1 && !isPrimeGeneric(candidate)) {
            /* Resize cache if necessary  */
            if (cache_size == cache_cap) {
                cache_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                /* In the unlikely event that realloc fails, abort */
                if (!tmp) {
                    abort();
                }
                cache = tmp;
            }
            cache[cache_size++] = candidate;
        }
        next_k++;
    }

    /* Return the (n-1)-th element of cache (0‑based index) */
    return cache[n - 1];
}
