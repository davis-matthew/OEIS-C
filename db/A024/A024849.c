/*
 * A024849  a(n) = least m such that if r and s in {|F(h+1)-tau*F(h)|: h =
 *          1,2,...,n} satisfy r < s, then r < k/m < s for some integer k,
 *          where F = A000045 (Fibonacci numbers) and tau = (1+sqrt(5))/2
 *          (golden ratio).
 * https://oeis.org/A024849
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A024849
   Recurrence:  a(n) = a(n‑1) + 2*a(n‑2) − a(n‑3) − a(n‑4)   for n > 7
   Initial values (offset 2): a(2)=2, a(3)=4, a(4)=6, a(5)=9,
   a(6)=14, a(7)=23
   The function computes a(n) for any n ≥ 2 using memoisation. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* For indices below the offset, the return value is irrelevant. */
    if (n < 2) return (seq_t)0;

    /* Static cache for memoisation.  cache[i] holds a(i).  */
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;   /* allocated size (in indices)   */
    static idx_t  last  = 1;   /* highest index that has been
                                 initialised (>=1)           */

    /* Initialise the cache on first call. */
    if (!cache) {
        cap   = 16;                  /* start with 16 slots           */
        cache = (seq_t *)calloc(cap, sizeof(seq_t)); /* conversion needed for C++ */
        /* initialise safe indices 0 and 1 (unused) */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
    }

    /* Ensure the cache is large enough.  Grow geometrically if required. */
    if (n + 1 > cap) {
        while (n + 1 > cap)
            cap <<= 1;                 /* double size until big enough */
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }

    /* Store initial values (if they haven't been stored already). */
    if (last < 7) {
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)6;
        cache[5] = (seq_t)9;
        cache[6] = (seq_t)14;
        cache[7] = (seq_t)23;
        last = 7;
    }

    /* Compute missing terms up to the requested index. */
    while (last < n) {
        ++last;
        /* a(last) = a(last-1) + 2*a(last-2) - a(last-3) - a(last-4) */
        seq_t term = cache[last-1];
        term += (seq_t)2 * cache[last-2];
        term -= cache[last-3];
        term -= cache[last-4];
        cache[last] = term;
    }

    return cache[n];
}
