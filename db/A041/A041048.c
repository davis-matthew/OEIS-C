/*
 * A041048  Numerators of continued fraction convergents to sqrt(30).
 * https://oeis.org/A041048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 22*a(n-2) - a(n-4)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0) = 5, a(1) = 11, a(2) = 115, a(3) = 241 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cached_len = 0;

seq_t a(idx_t n)
{
    /* Initialize cache with first four terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 5;
        cache[1] = 11;
        cache[2] = 115;
        cache[3] = 241;
        cached_len = 4;
    }

    if (n + (idx_t)1 > cached_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        /* Compute missing terms using recurrence */
        for (idx_t i = cached_len; i <= n; ++i) {
            cache[i] = 22 * cache[i - 2] - cache[i - 4];
        }
        cached_len = new_len;
    }

    return cache[n];
}
