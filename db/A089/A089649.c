/*
 * A089649  a(1)=1, a(2)=2, a(n) = a(n-1) + a(floor((n+1)/3)).
 * https://oeis.org/A089649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(floor((n+1)/3)), a(1)=1, a(2)=2
 * Range:     u8 n <= 31, u16 n <= 173, u32 n <= 1780, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation.  cache[0] is unused. */
    static seq_t *cache = NULL;
    static idx_t cached = 0;  /* largest index computed so far */

    if (n == 0) {
        /* Undefined for n < 1 – return 0. */
        return (seq_t)0;
    }

    /* Expand the cache if needed. */
    if (n > cached || cache == NULL) {
        /* Allocate space for 0..n (index 0 unused). */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (tmp == NULL) {
            /* Allocation failed – return 0. */
            return (seq_t)0;
        }
        cache = tmp;

        /* Ensure the initial values are set. */
        if (n >= 1) cache[1] = (seq_t)1;
        if (n >= 2) cache[2] = (seq_t)2;

        /* Compute new terms up to n. */
        idx_t start = 3;
        if (cached + 1 > start) start = cached + 1;
        for (idx_t i = start; i <= n; ++i) {
            idx_t j = (i + 1) / 3;          /* floor((i+1)/3) */
            cache[i] = cache[i - 1] + cache[j];
        }
        cached = n;
    }

    return cache[n];
}
