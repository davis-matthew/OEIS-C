/*
 * A051403  a(n) = (n+2)*(a(n-1) - a(n-2)), starting with a(-1)=0 and a(0)=1.
 * https://oeis.org/A051403
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+2) * (a(n-1) - a(n-2)), a(-1)=0, a(0)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 329 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* External integer types (supplied by the build) */
extern seq_t *alloc_seq_array(size_t n);

/* Return a(n) for n >= -1. */
seq_t a(idx_t n)
{
    /* Map n to array index: idx = n+1  (a(-1)->0, a(0)->1, ...) */
    if (n < -1) return 0; /* undefined for < -1, not used in tests */

    static seq_t *cache = NULL;      /* array of computed values */
    static idx_t cache_max = -1;     /* maximum n for which cache[n] is filled */

    /* Initialisation for n = -1 and n = 0 */
    if (cache == NULL) {
        cache = mem_new(seq_t, ((size_t)2));
        if (!cache) abort();        /* out of memory */
        cache[0] = (seq_t)0;        /* a(-1) */
        cache[1] = (seq_t)1;        /* a(0)  */
        cache_max = 0;
    }

    /* Extend cache if needed */
    if (n > cache_max) {
        size_t new_size = (size_t)(n + 2);           /* need indices 0 .. n+1 */
        cache = mem_resize(seq_t, cache, new_size);
        if (!cache) abort();

        /* Compute values for k = cache_max+2 .. n+1 */
        for (idx_t k_index = cache_max + 2; k_index <= n + 1; ++k_index) {
            /* k_index == current index in array = n+1 */
            /* multiplier = n+2 = k_index+1 */
            seq_t multiplier = (seq_t)(k_index + 1);
            seq_t diff = cache[k_index - 1] - cache[k_index - 2];
            cache[k_index] = multiplier * diff;
        }
        cache_max = n;
    }

    return cache[n + 1];
}
