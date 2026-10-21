/*
 * A224257  Number of n X 3 0..2 arrays with rows, diagonals and antidiagonals
 *          unimodal and columns nondecreasing.
 * https://oeis.org/A224257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 62, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   init_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A224257: Number of n×3 0..2 arrays with rows, diagonals and
   antidiagonals unimodal and columns nondecreasing.
   Recurrence (n > 7):
     a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5)
           - 7*a(n-6) + a(n-7)
   Initial values:
     a(1)=22, a(2)=158, a(3)=648, a(4)=2017, a(5)=5246,
     a(6)=11990, a(7)=24842
*/

#include <stdlib.h>
#include "../utils/init_term.h"

/* Static cache for memoisation */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* undefined for n < 1, return 0 as guard */

    /* Expand cache to include index n */
    if (cache_size <= n)
    {
        idx_t old_size  = cache_size;
        idx_t new_size  = n + 1;    /* indices 0 … n inclusive */

        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return 0;               /* allocation failure – return 0 */

        cache = tmp;

        /* Ensure cache[0] is defined to simplify recurrence */
        if (old_size == 0)
            cache[0] = 0;

        /* Initialise newly added entries */
        for (idx_t i = old_size; i < new_size; ++i)
        {
            if (i >= 1 && i <= 7)
            {
                cache[i] = init_term(i);
            }
            else if (i > 7)
            {
                /* Recurrence for n > 7 */
                seq_t v  = 7   * cache[i-1]
                           - 21 * cache[i-2]
                           + 35 * cache[i-3]
                           - 35 * cache[i-4]
                           + 21 * cache[i-5]
                           -  7 * cache[i-6]
                           +      cache[i-7];
                cache[i] = v;
            }
            else /* i == 0 */
            {
                cache[i] = 0;
            }
        }

        cache_size = new_size;
    }

    return cache[n];
}
