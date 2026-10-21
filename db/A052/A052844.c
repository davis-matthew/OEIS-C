/*
 * A052844  E.g.f.: exp(x*(2-x)/(1-x)).
 * https://oeis.org/A052844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = recurrence a(0)=1, a(1)=2, a(2)=6, a(n+3) =
 *            (2+n^2+3*n)*a(n) - (n^2+5*n+6)*a(n+1) + (2*n+6)*a(n+2) for n≥0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ==========================================================================
   The function follows the 3‑term linear recurrence above.
   This gives O(n) time per call (after any previous values are cached)
   and only adds a few machine words per step.
   The cache is extended on demand, so calls may come in any order.
   -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;          /* array of values */
    static idx_t cache_size = 0;          /* current allocated size */

    /* If we already have the value, just return it */
    if (cache && n < cache_size)
        return cache[n];

    /* Initialise cache with the first three terms if not yet done */
    if (!cache) {
        cache_size = 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
    }

    /* If required size exceeds current, grow and compute new terms */
    if (n + 1 > cache_size) {
        /* grow to at least the needed size, doubling for efficiency */
        idx_t new_size = cache_size * 2;
        if (new_size < n + 1)
            new_size = n + 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);

        /* compute terms from current size up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* index n in the recurrence is i-3 */
            idx_t ii = i - 3;
            seq_t cin = (seq_t)ii;          /* n as seq_t */

            seq_t coeff1 = (seq_t)2 + cin * cin + (seq_t)3 * cin;
            seq_t coeff2 = cin * cin + (seq_t)5 * cin + (seq_t)6;
            seq_t coeff3 = (seq_t)2 * cin + (seq_t)6;

            seq_t a_n   = cache[i - 3];
            seq_t a_n1  = cache[i - 2];
            seq_t a_n2  = cache[i - 1];

            cache[i] = (coeff1 * a_n + coeff3 * a_n2) - coeff2 * a_n1;
        }

        cache_size = n + 1;
    }

    return cache[n];
}
