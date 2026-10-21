/*
 * A208903  The sum over all bitstrings b of length n with at least two runs
 *          of the number of runs in b not immediately followed by a longer
 *          run.
 * https://oeis.org/A208903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A208903
   Recurrence: a(1)=0, a(2)=4, a(3)=12, a(4)=32, a(5)=76
                a(n)=5*a(n-1)-6*a(n-2)-6*a(n-3)+16*a(n-4)-8*a(n-5)   for n≥6
*/

#include <stdlib.h>

/* static cache for memoisation (1‑based) */
static seq_t *cache = NULL;
static idx_t cached_n = 0;   /* highest index stored in cache */

/* return a(n) */
seq_t a(idx_t n)
{
    /* Forward‑compatible small‑n handling */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)12;
    if (n == 4) return (seq_t)32;
    if (n == 5) return (seq_t)76;

    /* Ensure cache is large enough */
    if (!cache || n > cached_n)
    {
        size_t needed = (size_t)n + 1;        /* 1‑based indexing   */
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failure */

        cache = tmp;

        /* initialise first five entries if not yet done */
        if (cached_n < 5)
        {
            cache[1] = (seq_t)0;
            cache[2] = (seq_t)4;
            cache[3] = (seq_t)12;
            cache[4] = (seq_t)32;
            cache[5] = (seq_t)76;
            cached_n = 5;
        }

        /* grow cache and compute missing values */
        for (idx_t i = cached_n + 1; i <= n; ++i)
        {
            seq_t term = (seq_t)5 * cache[i-1]
                       - (seq_t)6 * cache[i-2]
                       - (seq_t)6 * cache[i-3]
                       + (seq_t)16 * cache[i-4]
                       - (seq_t)8 * cache[i-5];
            cache[i] = term;
        }
        cached_n = n;
    }
    return cache[n];
}
