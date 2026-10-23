/*
 * A164856  Row sums of generalized Lucas-Pascal triangle: A164855.
 * https://oeis.org/A164856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 101*a(n-2) - 100*a(n-3) - 100*a(n-4) initial
 *            values: a(0)=1, a(1)=102, a(2)=103, a(3)=10204
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* direct return for the first four terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)102;
    if (n == 2) return (seq_t)103;
    if (n == 3) return (seq_t)10204;

    /* initialise cache to hold the first four terms */
    if (cache_size == 0) {
        cache_size = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return 0;                /* out of memory – unlikely in tests */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)102;
        cache[2] = (seq_t)103;
        cache[3] = (seq_t)10204;
    }

    /* grow cache if necessary */
    if (n >= cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – just return 0 for safety */
            return 0;
        }
        cache = tmp;
        for (idx_t k = cache_size; k <= n; ++k) {
            cache[k] = cache[k-1]
                     + (seq_t)101 * cache[k-2]
                     - (seq_t)100 * cache[k-3]
                     - (seq_t)100 * cache[k-4];
        }
        cache_size = new_size;
    }

    return cache[n];
}
