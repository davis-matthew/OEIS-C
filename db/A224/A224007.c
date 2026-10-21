/*
 * A224007  Number of n X 3 0..2 arrays with rows nondecreasing and
 *          antidiagonals unimodal.
 * https://oeis.org/A224007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A224007: Number of n×3 0..2 arrays with rows nondecreasing
   and antidiagonals unimodal.
   Recurrence (for n ≥ 6):
     a(n) = 10*a(n-1) - 16*a(n-2) + 30*a(n-3) - 14*a(n-4) + 12*a(n-5)
   Initial values:
     a(1)=10, a(2)=100, a(3)=868, a(4)=7378, a(5)=62764
*/

#include <stdlib.h>   /* malloc, free, realloc */

seq_t a(idx_t n)
{
    /* static memoisation ---------------------------------------------------- */
    static idx_t cached_max = 0;   /* highest index computed so far          */
    static seq_t *cache = NULL;    /* 1‑based array holding values            */

    /* ensure cache big enough ---------------------------------------------- */
    if (n > cached_max)
    {
        /* grow the cache to size n (index 0 unused) */
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (tmp == NULL) abort();   /* allocation failure */
        cache = tmp;

        /* initialise new entries using the recurrence --------------------- */
        for (idx_t i = cached_max + 1; i <= n; ++i)
        {
            switch (i)
            {
                case 1: cache[i] = (seq_t)10;          break;
                case 2: cache[i] = (seq_t)100;         break;
                case 3: cache[i] = (seq_t)868;          break;
                case 4: cache[i] = (seq_t)7378;         break;
                case 5: cache[i] = (seq_t)62764;        break;
                default:
                {
                    /* a(i) = 10*a(i-1) - 16*a(i-2) + 30*a(i-3)
                           - 14*a(i-4) + 12*a(i-5) */
                    seq_t val = (seq_t)10 * cache[i-1];
                    val -= (seq_t)16 * cache[i-2];
                    val += (seq_t)30 * cache[i-3];
                    val -= (seq_t)14 * cache[i-4];
                    val += (seq_t)12 * cache[i-5];
                    cache[i] = val;
                }
            }
        }
        cached_max = n;
    }
    return cache[n];
}
