/*
 * A104204  If n==0 (mod 3) then a(n)=a(n-1); if n==1 (mod 3) then
 *          a(n)=a(n-2)+a(n-3); if n==2 (mod 3) then
 *          a(n)=a(n-3)+a(n-4)+a(n-5).
 * https://oeis.org/A104204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 39, u32 n <= 81, u64 n <= 163,
 *            u128 n <= 327, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A104204 – Triangle‐like recurrence
   a(0)=1, a(1)=1, a(2)=2
   if n%3==0  a(n)=a(n-1)
   if n%3==1  a(n)=a(n-2)+a(n-3)
   if n%3==2  a(n)=a(n-3)+a(n-4)+a(n-5) */
#include <stddef.h>
#include <stdlib.h>

/*  Prefilled cache up to 9 to allow the recurrence without loss of generality.
    The cache is expanded lazily on demand and memoised for subsequent calls. */
seq_t a(idx_t n)
{
    /* static cache state */
    static seq_t *cache   = NULL;   /* pointer to allocated cache array   */
    static size_t  cap     = 0;     /* current capacity of the array      */
    static size_t  last    = (size_t)-1;   /* index of the last computed term   */

    /* -- initialisation ----------------------------------------------------- */
    if (cap == 0) {
        /* allocate initial space (may grow later) */
        cap = 10;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation error – return 0 */

        /* base terms, as given by the OEIS entry */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)5;
        cache[5] = (seq_t)4;

        /* precompute terms 6 … 9 (they are internally needed for later terms) */
        for (size_t i = 6; i < cap; ++i) {
            if (i % 3 == 0)
                cache[i] = cache[i-1];
            else if (i % 3 == 1)
                cache[i] = cache[i-2] + cache[i-3];
            else
                cache[i] = cache[i-3] + cache[i-4] + cache[i-5];
        }
        last = cap - 1;          /* last computed index         */
    }

    /* -- extend cache if the requested index is not yet computed ---------------- */
    if ((size_t)n > last) {
        /* Ensure capacity is large enough, growing geometrically to keep it O(1) amortised */
        size_t needed = (size_t)n + 1;
        if (needed > cap) {
            size_t newcap = cap;
            while (newcap < needed) newcap <<= 1;  /* double until sufficient */
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;  /* realloc failure: return 0 */
            cache = tmp;
            cap   = newcap;
        }

        /* compute all missing terms up to n */
        for (size_t i = last + 1; i <= (size_t)n; ++i) {
            if (i % 3 == 0)
                cache[i] = cache[i-1];
            else if (i % 3 == 1)
                cache[i] = cache[i-2] + cache[i-3];
            else
                cache[i] = cache[i-3] + cache[i-4] + cache[i-5];
        }
        last = (size_t)n;   /* update last computed index */
    }

    return cache[(size_t)n];
}
