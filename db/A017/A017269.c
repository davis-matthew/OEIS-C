/*
 * A017269  Expansion of 1/((1-3x)(1-5x)(1-6x)).
 * https://oeis.org/A017269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - 63*a(n-2) + 90*a(n-3); with a(0)=1, a(1)=14,
 *            a(2)=133
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* malloc, realloc */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last_computed = (idx_t)-1;

    /* Initialize base values once */
    if (last_computed == (idx_t)-1) {
        if (cap < 3) {
            seq_t *tmp = (seq_t *)realloc(cache, 3 * sizeof(seq_t));
            cache = tmp != NULL ? tmp : cache;
            cap = 3;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)14;
        cache[2] = (seq_t)133;
        last_computed = 2;
    }

    if (n <= last_computed) {
        return cache[(size_t)n];
    }

    /* Ensure the cache can hold up to index n */
    if ((size_t)n >= cap) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        /* Assume allocation succeeds as per problem guarantees */
        cache = tmp != NULL ? tmp : cache;
        cap = (size_t)(n + 1);
    }

    /* Compute missing terms */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[(size_t)i] =
            (seq_t)(
                ((seq_t)14 * cache[(size_t)(i - 1)]) -
                ((seq_t)63 * cache[(size_t)(i - 2)]) +
                ((seq_t)90 * cache[(size_t)(i - 3)])
            );
    }
    last_computed = n;

    return cache[(size_t)n];
}
