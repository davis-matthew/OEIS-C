/*
 * A042123  Denominators of continued fraction convergents to sqrt(586).
 * https://oeis.org/A042123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued fraction convergents to sqrt(586)

   The sequence satisfies
        a(0) = 1
        a(1) = 4
        a(n) = coeff[(n-1) % 23] * a(n-1) + a(n-2)   (n ≥ 2)

   where the periodic coefficients are the partial quotients of
   the continued fraction of sqrt(586) after the initial floor:
        coeff =  [4,1,4,1,1,2,1,2,7,1,2,2,1,7,2,1,2,1,1,4,1,4,48]  (period 23).
   The coefficients repeat every 23 terms.  This recurrence produces
   all terms listed in OEIS A042123. */

#include <stdlib.h>

static const seq_t coeff[23] = {
    (seq_t)4,  (seq_t)1,  (seq_t)4,  (seq_t)1,  (seq_t)1,  (seq_t)2,
    (seq_t)1,  (seq_t)2,  (seq_t)7,  (seq_t)1,  (seq_t)2,  (seq_t)2,
    (seq_t)1,  (seq_t)7,  (seq_t)2,  (seq_t)1,  (seq_t)2,  (seq_t)1,
    (seq_t)1,  (seq_t)4,  (seq_t)1,  (seq_t)4,  (seq_t)48
};

seq_t a(idx_t n)
{
    /* Use a simple growing cache so repeated calls are fast. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Ensure at least the first two terms are available. */
    if (cache_len == 0) {
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache_len = 2;
    }

    /* Return cached value if possible. */
    if (n < cache_len) {
        return cache[n];
    }

    /* Extend cache up to requested index. */
    size_t required = (size_t)n + 1;
    if (required > cache_len) {
        seq_t *new_cache = (seq_t*)realloc(cache, required * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;      /* allocation failure */
        cache = new_cache;

        for (size_t i = cache_len; i < required; ++i) {
            seq_t coef = coeff[(i - 1) % 23];
            cache[i] = coef * cache[i - 1] + cache[i - 2];
        }
        cache_len = required;
    }

    return cache[n];
}
