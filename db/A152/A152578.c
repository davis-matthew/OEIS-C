/*
 * A152578  a(n) = 5^(2^(n-1)) + 2.
 * https://oeis.org/A152578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(2^(n-1)) + 2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) using the recurrence:
 *   a(1) = 7
 *   a(n) = (a(n-1) - 2)^2 + 2   for n > 1
 * to avoid explicit exponentiation of enormous numbers. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* cache[1]..cache[cache_size-1] are valid */

    if (n == 0) return (seq_t)0;   /* safety for out-of‑range indices */

    if (n >= cache_size) {
        /* grow cache to hold up to n */
        size_t new_size = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 1) {
                cache[1] = (seq_t)7;          /* 5^1 + 2 */
            } else {
                seq_t t = cache[i-1] - (seq_t)2; /* 5^(2^(i-2)) */
                cache[i] = t * t + (seq_t)2;     /* square + 2 */
            }
        }
        cache_size = n + 1;
    }
    return cache[n];
}
