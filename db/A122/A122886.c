/*
 * A122886  Interlaced merger of A122883, A122884 and A122885.
 * https://oeis.org/A122886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-3) + 11*a(n-6) - 2*a(n-9) (Initial values: a(0)=0,
 *            a(1)=0, a(2)=1, a(3)=1, a(4)=1, a(5)=1, a(6)=3, a(7)=7, a(8)=13)
 * Range:     u8 n <= 13, u16 n <= 22, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* a memoized implementation that works for any seq_t supporting
   integer arithmetic (including an arbitrary‑precision class) */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    /* Base values for the first nine terms */
    static const seq_t base[9] = {
        (seq_t)0,  /* a(0) */
        (seq_t)0,  /* a(1) */
        (seq_t)1,  /* a(2) */
        (seq_t)1,  /* a(3) */
        (seq_t)1,  /* a(4) */
        (seq_t)1,  /* a(5) */
        (seq_t)3,  /* a(6) */
        (seq_t)7,  /* a(7) */
        (seq_t)13  /* a(8) */
    };

    /* If already computed, return the value */
    if (n < cache_cap) {
        return cache[n];
    }

    /* Extend the cache to include up to n */
    idx_t new_cap = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();    /* allocation failure */
    cache = tmp;

    /* Compute new terms */
    for (idx_t i = cache_cap; i < new_cap; ++i) {
        if (i < 9) {
            cache[i] = base[i];
        } else {
            /* use the recurrence with seq_t arithmetic */
            seq_t term = cache[i - 3] * 4
                       + cache[i - 6] * 11
                       - cache[i - 9] * 2;
            cache[i] = term;
        }
    }

    cache_cap = new_cap;
    return cache[n];
}
