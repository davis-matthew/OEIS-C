/*
 * A063089  Dimension of the space of weight 2n cusp forms for Gamma_0( 21 ).
 * https://oeis.org/A063089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) for n>5 a(1)=1, a(2)=6, a(3)=12,
 *            a(4)=16, a(5)=22
 * Range:     u8 n <= 48, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow arbitrary order calls */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;

    /* handle the first five terms directly */
    if (n <= 5) {
        /* indices 1..5 are significant; cache[0] unused */
        const seq_t init[6] = {0, 1, 6, 12, 16, 22};
        return init[n];
    }

    /* extend cache if necessary */
    if (n > max_computed) {
        /* allocate new space that fits indices 0..n */
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) { /* allocation failed – return 0 (failsafe) */
            return (seq_t)0;
        }
        cache = new_cache;

        /* initialize first five values if this is the first run */
        if (max_computed == 0) {
            cache[1] = 1;
            cache[2] = 6;
            cache[3] = 12;
            cache[4] = 16;
            cache[5] = 22;
            max_computed = 5;
        }

        /* fill remaining values up to n using the recurrence */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            cache[i] = cache[i - 1] + cache[i - 3] - cache[i - 4];
        }
        max_computed = n;
    }

    return cache[n];
}
