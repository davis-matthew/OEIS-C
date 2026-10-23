/*
 * A143759  Aliquot sequence starting at 86.
 * https://oeis.org/A143759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 86, a(n+1) = sum of proper divisors of a(n) (A001065)
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n-th term of the aliquot sequence starting at 86. */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t   cache_size = 0;

    /* initialise cache for the first call */
    if (cache_size == 0) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failed */
        cache[0] = (seq_t)86;
        cache_size = 1;
    }

    /* grow the cache up to n (inclusive) */
    if (cache_size <= n) {
        idx_t  new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;         /* allocation failed */
        cache = tmp;

        /* compute missing values */
        for (idx_t i = cache_size; i < new_size; ++i) {
            seq_t prev = cache[i - 1];
            seq_t sum = 0;

            if (prev > (seq_t)1) {
                /* start with divisor 1 */
                sum = (seq_t)1;
                seq_t d = (seq_t)2;
                /* loop over potential divisors up to sqrt(prev) */
                while (d * d <= prev) {
                    if (prev % d == (seq_t)0) {
                        seq_t other = prev / d;
                        sum += d;
                        if (other != d) sum += other;
                    }
                    d += (seq_t)1;
                }
            }
            /* for prev <= 1, sum stays 0 */
            cache[i] = sum;
        }
        cache_size = new_size;
    }

    return cache[n];
}
