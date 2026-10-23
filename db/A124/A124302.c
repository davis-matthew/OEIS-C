/*
 * A124302  Number of set partitions with at most 3 blocks; number of Dyck
 *          paths of height at most 4; dimension of space of symmetric
 *          polynomials in 3 noncommuting variables.
 * https://oeis.org/A124302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(0)=1, a(1)=1, a(2)=2, a(n)=4*a(n-1)-3*a(n-2) for n>=3
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to support out‑of‑order calls */
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;   /* number of filled entries      */
    static size_t cache_cap  = 0;   /* physical capacity of cache    */

    /* enlarge the cache when necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
        } else if (i == 1) {
            cache[1] = 1;
        } else if (i == 2) {
            cache[2] = 2;
        } else {
            seq_t term = cache[i - 1] * 4;
            term -= cache[i - 2] * 3;
            cache[i] = term;
        }
    }
    cache_size = n + 1;

    return cache[n];
}
