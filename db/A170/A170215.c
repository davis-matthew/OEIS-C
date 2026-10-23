/*
 * A170215  Number of reduced words of length n in Coxeter group on 14
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Recurrence derived from the rational generating function
   P(t) = t^40 + 2 t^39 + … + 2 t + 1
   Q(t) = 78 t^40 - 12 t^39 - … - 12 t + 1
   which gives
       a(n) = p_n + 12·∑_{i=1}^{39} a(n-i) – 78·a(n-40)   (n ≥ 0)
   with
       p_0 = 1 ,  p_40 = 1 ,  p_i = 2 for 1 ≤ i ≤ 39 ,
       p_i = 0 otherwise.
   For negative indices the terms a(k) are taken to be 0.
*/

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t  computed = 0;   /* number of terms already stored, i.e. size of cache */

    /* extend the cache if needed */
    if (!cache || computed <= n)
    {
        size_t new_size = (size_t)(n + 1);

        if (!cache)
        {
            cache = (seq_t*)calloc(new_size, sizeof(seq_t));
            computed = 0;
        }
        else if (computed < new_size)
        {
            cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        }

        /* compute missing terms from 'computed' up to 'n' inclusive */
        for (idx_t i = computed; i <= n; ++i)
        {
            seq_t p;
            if (i == 0)
                p = 1;
            else if (i == 40)
                p = 1;
            else if (i <= 39)
                p = 2;
            else
                p = 0;

            seq_t sum39 = 0;
            for (idx_t k = 1; k <= 39 && i >= k; ++k)
                sum39 += cache[i - k];

            seq_t term = p + 12 * sum39;
            if (i >= 40)
                term -= 78 * cache[i - 40];

            cache[i] = term;
        }

        computed = n + 1;
    }

    return cache[n];
}
