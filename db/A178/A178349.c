/*
 * A178349  Partial sums of Sum_{n>=0} 1010^n.
 * https://oeis.org/A178349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = a(n-1)*1010 + 1 for n > 1 (OEIS A178349)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_capacity.h"  /* provides void ensure_capacity(size_t, size_t*, void**, size_t) */

seq_t a(idx_t n) {
    /* Handle the unusual case n == 0; offset is 1, behaviour below offset is irrelevant. */
    if (n == 0) return (seq_t)0;

    /* Static cache for sequence values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated size of cache (including unused index 0) */
    static size_t max_n = 0;   /* highest n for which cache[n] is valid */

    /* Ensure sufficient capacity: we use an array indexed from 0..n, but ignore index 0. */
    size_t needed = (size_t)n + 1;                 /* plus one for zero index */
    if (cap < needed) {
        ensure_capacity(needed, &cap, (void **)&cache, sizeof(seq_t));
    }

    /* Base case initialisation */
    if (max_n == 0) {
        cache[1] = (seq_t)1;
        max_n   = 1;
    }

    /* Extend cache as needed using recurrence a(k) = a(k-1)*1010 + 1 */
    for (size_t i = max_n + 1; i <= (size_t)n; ++i) {
        cache[i] = cache[i - 1] * (seq_t)1010 + (seq_t)1;
    }
    max_n = (size_t)n;

    return cache[(size_t)n];
}
