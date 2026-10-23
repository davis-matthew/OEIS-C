/*
 * A164110  a(n) = 12*a(n-1) - 34*a(n-2) for n > 1; a(0) = 5, a(1) = 36.
 * https://oeis.org/A164110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 12*a(n-1) - 34*a(n-2), a(0) = 5, a(1) = 36.
 * The function uses a static cache to handle calls with arbitrary ordering.
 *
 * It works for primitive integral types as well as for an arbitrary-precision
 * integer class.  All arithmetic is performed in the native seq_t type, so
 * intermediate values are confined to the required range.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of computed values          */
    static idx_t cache_size = 0;     /* number of values stored           */
    static idx_t cache_cap  = 0;     /* capacity of the allocated array   */

    /* Ensure enough capacity for index n (including 0). */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        if (new_cap < 8) new_cap = 8;          /* small initial capacity */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute values from the first missing index up to n. */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)5;
        } else if (i == 1) {
            cache[1] = (seq_t)36;
        } else {
            cache[i] = (seq_t)12 * cache[i - 1] - (seq_t)34 * cache[i - 2];
        }
    }

    cache_size = n + 1;
    return cache[n];
}
