/*
 * A234596  Bicolored noncrossing configurations.
 * https://oeis.org/A234596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies 3*∑_{k=0}^n a(k)*a(n−k) + 4*a(n−1) + [n==1] +
 *            [n==2] = a(n) with a(0)=0, a(1)=1. (Offset 1: a(1)=1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cached_size = 0;      /* number of elements stored (size) */

    /* zero is never requested, but guard against it */
    if (n == 0) return 0;

    /* Convert idx_t to size_t for indexing */
    size_t idx = (size_t)n;

    /* Allocate and initialise cache if first call */
    if (cached_size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 2);
        if (!cache) return 0;          /* out of memory – caller must handle */
        cache[0] = (seq_t)0;           /* a(0) = 0 */
        cache[1] = (seq_t)1;           /* a(1) = 1 */
        cached_size = 2;
    }

    /* Expand cache to include requested index */
    if (idx >= cached_size) {
        size_t new_size = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;            /* out of memory – caller must handle */
        cache = tmp;

        /* Compute new values */
        for (size_t i = cached_size; i < new_size; ++i) {
            /* Convolution sum S_i = sum_{k=0}^i a_k * a_{i-k} */
            seq_t sum = (seq_t)0;
            for (size_t k = 0; k <= i; ++k) {
                sum += cache[k] * cache[i - k];
            }
            seq_t val = (seq_t)3 * sum + (seq_t)4 * cache[i - 1];
            if (i == 1) val += (seq_t)1;
            if (i == 2) val += (seq_t)1;
            cache[i] = val;
        }
        cached_size = new_size;
    }

    return cache[idx];
}
