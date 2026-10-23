/*
 * A185455  Trajectory of 7 under repeated application of the map in A185452.
 * https://oeis.org/A185455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=7; For n>1, a(n)=f(a(n-1)) where f(x) = x/2 if x is even,
 *            (5*x+1)/2 if x is odd
 * Range:     u8 n <= 8, u16 n <= 23, u32 n <= 124, u64 n <= 496,
 *            u128 n <= 757, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses simple memoisation so that multiple calls
   with arbitrary indices run in O(n) time for the maximum
   requested n.  All arithmetic is performed with the type
   seq_t, which may be 8/16/32/64/128‑bit unsigned or an
   arbitrary‑precision integer class. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values, indices start at 1 */
    static seq_t *cache = NULL;   /* cache[1] holds a(1)          */
    static idx_t  cached_up_to = 0; /* largest index computed      */

    if (n < 1)
        return 0; /* undefined for offsets below 1, unused            */

    /* Make sure the cache is large enough   */
    if (n > cached_up_to) {
        size_t new_size = (size_t)n + 1; /* +1 because we ignore index 0 */
        seq_t *tmp = (cache == NULL) ?
                     (seq_t*)malloc(new_size * sizeof(seq_t)) :
                     (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed – should not happen   */
        cache = tmp;

        /* If this is the first allocation, initialise a(1) */
        if (cached_up_to == 0) {
            cache[1] = 7;          /* a(1) = 7             */
            cached_up_to = 1;      /* we have computed a(1) */
        }

        /* Compute missing values up to n */
        for (idx_t i = cached_up_to + 1; i <= n; ++i) {
            seq_t prev = cache[(size_t)(i - 1)];
            seq_t next;
            if ((prev & 1) == 0)           /* even */
                next = prev / 2;
            else                           /* odd  */
                next = (prev * 5 + 1) / 2; /* (5*prev+1)/2 is exact */
            cache[(size_t)i] = next;
        }
        cached_up_to = n;
    }

    return cache[(size_t)n];
}
