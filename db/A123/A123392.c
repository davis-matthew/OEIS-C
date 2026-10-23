/*
 * A123392  a(-3) = a(-2) = a(-1) = 0, a(0) = 1, a(n) = a(n-1) + 2*a(n-2) +
 *          2*a(n-3) + a(n-4), for n>0.
 * https://oeis.org/A123392
 *
 * Offset:    -3    (a(-3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 2*a(n-3) + a(n-4) a(-3) = a(-2) =
 *            a(-1) = 0, a(0) = 1
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 53, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < -3) return 0;
    if (n < 0)  return 0;

    static seq_t *cache = NULL;
    static idx_t  cached_until = -4;   /* index of the largest computed term */

    if (cache == NULL) {
        cache = (seq_t*)calloc(4, sizeof(seq_t));   /* indices -3..0 */
        if (!cache) abort();
        cache[0] = 0; /* a(-3) */
        cache[1] = 0; /* a(-2) */
        cache[2] = 0; /* a(-1) */
        cache[3] = 1; /* a(0) */
        cached_until = 0;
    }

    if (n > cached_until) {
        size_t new_len = (size_t)n + 4;   /* length for indices -3..n */
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (idx_t k = cached_until + 1; k <= n; ++k) {
            size_t i = (size_t)(k + 3);   /* array position for index k */
            cache[i] = cache[i-1] + 2*cache[i-2] + 2*cache[i-3] + cache[i-4];
        }
        cached_until = n;
    }

    return cache[(size_t)n + 3];
}
