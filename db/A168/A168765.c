/*
 * A168765  Number of reduced words of length n in Coxeter group on 40
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^18 = I.
 * https://oeis.org/A168765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the given rational generating function:
   a(n) = 38·∑_{i=1}^{17} a(n-i) – 741·a(n-18) + coeff_N(n)
   with coeff_N(0)=1, coeff_N(1..17)=2, coeff_N(18)=1, coeff_N(n)=0 for n>18.
*/

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static idx_t  size   = 0;   /* size of allocated array */
    static idx_t  last   = (idx_t)-1; /* highest index computed so far */

    /* enlarge storage if necessary */
    if (n + 1 > size)
    {
        seq_t *new_cache = alloc_seq_array(n + 1);
        if (cache != NULL)
        {
            /* copy existing values */
            for (idx_t i = 0; i <= last && i <= n; ++i)
                new_cache[i] = cache[i];
            free_seq_array(cache);
        }
        /* initialise the new slots to zero (not strictly needed but safe) */
        for (idx_t i = last + 1; i <= n; ++i)
            new_cache[i] = (seq_t)0;
        cache = new_cache;
        size  = n + 1;
    }

    /* compute any terms that are still missing */
    for (idx_t k = last + 1; k <= n; ++k)
    {
        seq_t sum = (seq_t)0;
        /* Σ_{i=1}^{17} a(k-i) */
        for (idx_t i = 1; i <= 17 && i <= k; ++i)
            sum += cache[k - i];

        seq_t term = sum * (seq_t)38;   /* 38·sum */

        /* subtract 741·a(k-18) if k≥18 */
        if (k >= 18)
            term -= cache[k - 18] * (seq_t)741;

        /* add the coefficient from the numerator of the G.F. */
        if (k == 0)
            term += (seq_t)1;
        else if (k <= 17)
            term += (seq_t)2;
        else if (k == 18)
            term += (seq_t)1;
        /* else term += 0; */

        cache[k] = term;
    }
    last = n;
    return cache[n];
}
