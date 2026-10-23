/*
 * A037489  Base 4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1.
 * https://oeis.org/A037489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A037489
   Recurrence: a(n) = 4*a(n-1) + a(n-2) – 4*a(n-3)
   Initial values: a(1)=2, a(2)=9, a(3)=38
   Offset: 1
   (Also a(n) = ⌊3·4ⁿ / 5⌋ )
*/

#include <stdlib.h>  /* malloc, realloc */

seq_t a(idx_t n)
{
    /* undefined for n==0 in this sequence – return 0 safely */
    if (n == 0)
        return (seq_t)0;

    /* cache the already-computed terms; cache[i] holds a(i+1)   */
    static seq_t *cache   = NULL;
    static idx_t cache_len = 0;   /* highest index+1 that has been computed */
    static idx_t cache_cap = 0;   /* allocated array size */

    /* Grow cache if needed to hold at least n terms */
    if (n > cache_cap) {
        idx_t new_cap = (cache_cap == 0) ? 1 : cache_cap;
        while (new_cap < n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {  /* allocation error – fail gracefully */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize base cases the first time */
    if (cache_len == 0) {
        /* ensure we have capacity for the three initial terms */
        if (cache_cap < 3) {
            idx_t new_cap = (cache_cap == 0) ? 3 : cache_cap;
            while (new_cap < 3)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)9;
        cache[2] = (seq_t)38;
        cache_len = 3;
    } else if (cache_len < 3) {  /* if we were asked a very small n before all
                                 * base terms were filled (unlikely) */
        if (cache_len < 1)
            cache[0] = (seq_t)2;
        if (cache_len < 2)
            cache[1] = (seq_t)9;
        if (cache_len < 3)
            cache[2] = (seq_t)38;
        cache_len = 3;
    }

    /* Return a cached value if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* Compute the missing terms up to n */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t a1 = cache[i - 1];   /* a(i)   */
        seq_t a2 = cache[i - 2];   /* a(i-1) */
        seq_t a3 = cache[i - 3];   /* a(i-2) */
        cache[i] = (seq_t)4 * a1 + a2 - (seq_t)4 * a3;
    }

    cache_len = n;
    return cache[n - 1];
}
