/*
 * A041864  Numerators of continued fraction convergents to sqrt(454).
 * https://oeis.org/A041864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 18, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A041864: Numerators of continued fraction convergents to sqrt(454).
   The sequence satisfies the recurrence

        a(0) = 21
        a(1) = 64
        a(n) = cf[n-1] * a(n-1) + a(n-2)   for n ≥ 2

   where cf[i] (i ≥ 0) are the partial quotients of the continued fraction
   expansion of sqrt(454) with period 34:
        cf = {3,3,1,13,2,3,2,1,1,4,6,1,7,1,1,1,20,1,1,1,7,1,6,4,1,1,2,3,2,13,1,3,3,42}
   The period repeats, so cf[(n-1) % 34] is the coefficient used at step n.

   The values are computed lazily and cached, so each term is produced in O(1)
   time after the first computation of that index.
*/
#include <stdlib.h>
#include <stdint.h>

/* public function -------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache  = NULL;
    static idx_t  cache_len = 0;
    static idx_t  cache_cap = 0;

    /* partial quotients of the continued fraction of sqrt(454), period 34 */
    static const seq_t cf[34] = {
        3, 3, 1, 13, 2, 3, 2, 1, 1, 4,
        6, 1, 7, 1, 1, 1, 20, 1, 1, 1,
        7, 1, 6, 4, 1, 1, 2, 3, 2, 13,
        1, 3, 3, 42
    };
    const idx_t PERIOD = 34;

    /* Helper to grow cache.  Uses realloc in a simple growth policy. */
    if (cache == NULL) {
        cache_cap = 64;                     /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    if (cache_cap < n + 1) {
        cache_cap = (n + 1) * 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* initialise first term if necessary */
    if (cache_len == 0) {
        cache[0] = (seq_t)21;               /* a(0) */
        cache_len = 1;
    }

    /* compute and cache terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t term;
        if (i == 1) {
            /* a(1) = cf[0] * a(0) + 1  (since a(-1) = 1) */
            term = cf[0] * cache[0] + (seq_t)1;
        } else {
            /* a(i) = cf[(i-1)%PERIOD] * a(i-1) + a(i-2)  for i >= 2 */
            idx_t idx = (i - 1) % PERIOD;
            term = cf[idx] * cache[i - 1] + cache[i - 2];
        }
        cache[i] = term;
    }
    cache_len = n + 1;
    return cache[n];
}
