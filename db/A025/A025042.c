/*
 * A025042  Number of partitions of { 1, 2, ..., 10n } into sets of size 10.
 * https://oeis.org/A025042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) * binomial(10*n-1, 9), a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* static cache for successive computations */
    static seq_t *cache = NULL;
    static idx_t cached_up_to = 0;   /* largest n already stored */

    if (n == 0) return (seq_t)1;

    /* allocate or extend cache if necessary */
    size_t new_cap = (size_t)n + 1;
    if (!cache) {
        cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failed */
        cache[0] = (seq_t)1;
        cached_up_to = 0;
    } else if (new_cap > cached_up_to + 1) {
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;
    }

    /* compute missing values up to n */
    for (idx_t k = cached_up_to + 1; k <= n; ++k) {
        seq_t bin = binomial(10*k - 1, (idx_t)9);
        cache[k] = cache[k-1] * bin;
    }
    cached_up_to = n;

    return cache[n];
}
