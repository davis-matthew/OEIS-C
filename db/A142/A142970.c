/*
 * A142970  Numerators of n-th approximants of a continued fraction for Pi-3.
 * https://oeis.org/A142970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac, cofr
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   alloc_seq_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A142970 – Numerators of n-th approximants of a continued fraction for Pi-3
 *
 * Formula: a(0) = 0, a(1) = 1
 *          a(n) = 6*a(n-1) + ((2*n-1)^2)*a(n-2)     for n >= 2
 *
 * (All operations are performed on the integer type seq_t.)
 */

#include <stdint.h>
#include <stdlib.h>
/* Utility functions for dynamic allocation of seq_t arrays (work for both built‑in
 * and arbitrary‑precision seq_t types). */
#include "../utils/alloc_seq_array.h"   /* returns seq_t* */
#include "../utils/seq_free_array.h"    /* frees a seq_t* array */

seq_t a(idx_t n)
{
    /* Base cases – handled without caching. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialise the cache on first use. */
    if (cache == NULL) {
        cache_size = 2;
        cache = alloc_seq_array(cache_size);
        if (cache == NULL) return (seq_t)0;   /* allocation failure → return 0 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    }

    /* Grow the cache up to index n if necessary. */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *new_cache = alloc_seq_array(new_size);
        if (new_cache == NULL) return (seq_t)0;  /* allocation failure */

        /* Copy existing values. */
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];

        /* Compute missing terms. */
        for (idx_t i = cache_size; i < new_size; ++i) {
            seq_t two_minus_one = (seq_t)(2 * i - 1);
            seq_t m2 = two_minus_one * two_minus_one;
            new_cache[i] = (seq_t)6 * new_cache[i - 1] + m2 * new_cache[i - 2];
        }

        /* Free the old array and update the cache. */
        seq_free_array(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
