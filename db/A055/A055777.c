/*
 * A055777  a(n) = 3^(3^n).
 * https://oeis.org/A055777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)^3, a(0) = 3
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* ensure cache can hold requested index */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 1 : cache_cap;
        while (new_cap <= (size_t)n) {
            new_cap *= 2;
        }
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* initialize first term */
    if (cache_len == 0) {
        cache[0] = (seq_t)3;   /* a(0) = 3 */
        cache_len = 1;
    }

    /* compute missing terms up to n */
    while (cache_len <= (size_t)n) {
        seq_t prev = cache[cache_len - 1];
        seq_t sq   = prev * prev;      /* prev^2 */
        seq_t cub  = sq * prev;        /* prev^3 */
        cache[cache_len] = cub;
        ++cache_len;
    }

    return cache[(size_t)n];
}
