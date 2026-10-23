/*
 * A014882  a(1) = 1, a(n) = 12*a(n-1) + n.
 * https://oeis.org/A014882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) + n, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;         /* cache[0] holds a(1) */
    static size_t cached_cap = 0;       /* number of terms currently cached */

    if (n == 0) return (seq_t)0;        /* out of bounds */

    if ((size_t)n > cached_cap) {
        /* Grow cache to hold terms up to n */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* allocation failed */
        cache = tmp;

        /* Initialize the first term if this is the first call */
        if (cached_cap == 0) {
            cache[0] = (seq_t)1;       /* a(1) */
        }

        /* Compute missing terms */
        size_t start = (cached_cap == 0) ? 1 : cached_cap;
        for (size_t i = start; i < (size_t)n; ++i) {
            /* i corresponds to a(i+1) */
            cache[i] = cache[i-1] * (seq_t)12 + (seq_t)(i + 1);
        }

        cached_cap = (size_t)n;
    }

    return cache[(size_t)n - 1];
}
