/*
 * A219508  Pierce expansion of 4*(3 - 2*sqrt(2)).
 * https://oeis.org/A219508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 3 For n ≥ 1: a(2*n) = 4 * (a(2*n-1) + 1) For n
 *            ≥ 1: a(2*n+1) = 2 * a(2*n-1)^2 – 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* dynamic cache of previously computed terms */
    static size_t cache_size = 0; /* number of elements currently stored (>= cache_size) */

    /* Grow cache if necessary */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failed – return zero (unlikely in tests) */
            return (seq_t)0;
        }
        cache = tmp;
        /* Compute only the new entries; existing entries remain valid */
        for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)3;
            } else if ((i & 1) == 0) {            /* even index: 2*k */
                seq_t prev = cache[i - 1];
                cache[i] = (seq_t)4 * (prev + (seq_t)1);
            } else {                             /* odd index: 2*k+1 */
                seq_t prev_odd = cache[i - 2];
                cache[i] = (seq_t)2 * prev_odd * prev_odd - (seq_t)1;
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
