/*
 * A131601  The number of different configurations of an n-block of a shift
 *          space with k symbols where each symbol but the first must appear
 *          isolated and separated from others by an block of length at least
 *          m made of first symbol. Here k=49 and m=2.
 * https://oeis.org/A131601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 48*a(n-3); initial values: a(1) = 49, a(2) = 97,
 *            a(3) = 145
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle indices below the offset directly */
    if (n == 1) return 49;
    if (n == 2) return 97;
    if (n == 3) return 145;

    /* 1‑based cache of already computed values */
    static seq_t *cache   = NULL;   /* array of seq_t */
    static idx_t  cap     = 0;      /* current allocated size (indices 0..cap-1) */
    static idx_t  maxcomp = 0;      /* highest index that has been computed */

    /* Ensure we have enough space for indices up to n (inclusive) */
    if ((idx_t)n >= cap) {
        idx_t newcap = (idx_t)n + 1;                     /* indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp && newcap > 0) return 0;              /* allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* Initialize base values on first use or after a resize that lost them */
    if (maxcomp == 0) {
        cache[1] = 49;
        cache[2] = 97;
        cache[3] = 145;
        maxcomp = 3;
    }

    /* Compute any missing terms up to n */
    if (n > maxcomp) {
        for (idx_t i = maxcomp + 1; i <= n; ++i) {
            /* recurrence: a(i) = a(i-1) + 48*a(i-3) */
            cache[i] = cache[i - 1] + 48 * cache[i - 3];
        }
        maxcomp = n;
    }

    return cache[n];
}
