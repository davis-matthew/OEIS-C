/*
 * A058278  Expansion of (1 - x^2)/(1 - x - x^3).
 * https://oeis.org/A058278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) (n ≥ 3), a(0) = 1, a(1) = 1, a(2) = 0
 * Range:     u8 n <= 17, u16 n <= 31, u32 n <= 60, u64 n <= 118,
 *            u128 n <= 235, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache      = NULL;     /* array holding a(0)...a(m) */
    static idx_t  cap       = 0;         /* allocated size of cache      */
    static idx_t  last_computed = (idx_t)-1; /* highest index that has been filled */

    /* ensure that we have space for at least indices 0..2 */
    if (cap < 3) {
        /* grow to at least 4 to avoid repeatedly reallocating */
        idx_t new_cap = 4;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) abort();            /* allocation failure: give up */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)0;
        cap = new_cap;
        last_computed = 2;
    }

    /* if required index is beyond current allocation, grow */
    if (n >= cap) {
        idx_t new_cap = cap;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) abort();
        cap = new_cap;
    }

    /* compute new entries up to n, if any */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            /* recurrence: a(i) = a(i-1) + a(i-3) */
            cache[i] = cache[i-1] + cache[i-3];
        }
        last_computed = n;
    }

    return cache[n];
}
