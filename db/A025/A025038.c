/*
 * A025038  Number of partitions of { 1, 2, ..., 6n } into sets of size 6.
 * https://oeis.org/A025038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((2n-1)(3n-1)(3n-2)(6n-1)(6n-5)/10) * a(n-1), a(0)=1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cached_cnt = 0;

    if (cache == NULL) {
        /* Allocate storage for the first n+1 terms. */
        cache = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;
        cache[0] = (seq_t)1;
        cached_cnt = 1;
    }

    if (cached_cnt <= n) {
        /* Grow the cache if needed. */
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;
        cache = tmp;

        /* Compute any missing values up to index n. */
        for (idx_t k = cached_cnt; k <= n; ++k) {
            seq_t prev = cache[k - 1];

            seq_t f1 = (seq_t)(2 * k - 1);
            seq_t f2 = (seq_t)(3 * k - 1);
            seq_t f3 = (seq_t)(3 * k - 2);
            seq_t f4 = (seq_t)(6 * k - 1);
            seq_t f5 = (seq_t)(6 * k - 5);

            seq_t term = f1 * f2;
            term *= f3;
            term *= f4;
            term *= f5;
            term /= (seq_t)10;  /* exact division */

            cache[k] = prev * term;
        }

        cached_cnt = n + 1;
    }

    return cache[n];
}
