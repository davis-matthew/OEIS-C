/*
 * A017860  Expansion of g.f. 1/(1 - x^7 - x^8 - x^9 - x^10 - x^11).
 * https://oeis.org/A017860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=a(n-7)+a(n-8)+a(n-9)+a(n-10)+a(n-11) for n>10; initial
 *            values: a(0)=1, a(1..6)=0, a(7..11)=1
 * Range:     u8 n <= 41, u16 n <= 72, u32 n <= 133, u64 n <= 254,
 *            u128 n <= 497, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* MEMOIZED DP implementation.  Works for any n>=0, called in any order. */
#include <stdlib.h>

static seq_t *cache = NULL;   /* array of computed values */
static idx_t cache_cap = 0;   /* size of array allocated */

/* Number of compositions of n into parts 7..11 */
seq_t a(idx_t n)
{
    /* Allocate / expand cache to cover index n */
    if (cache_cap <= n)
    {
        idx_t new_cap = n + 1;                          /* one element per index */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                             /* allocation failure */
        cache = tmp;

        /* Initialise or compute new entries */
        for (idx_t i = cache_cap; i <= n; ++i)
        {
            if (i == 0)                 cache[i] = 1;
            else if (i < 7)             cache[i] = 0;
            else if (i <= 11)           cache[i] = 1;
            else                        /* recurrence for i >= 12 */
                cache[i] = cache[i-7] + cache[i-8] + cache[i-9]
                           + cache[i-10] + cache[i-11];
        }
        cache_cap = new_cap;
    }
    return cache[n];
}
