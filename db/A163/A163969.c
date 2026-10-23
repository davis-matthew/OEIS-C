/*
 * A163969  Number of reduced words of length n in Coxeter group on 20
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A163969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A163969
   Recurrence (from the generating function):
   a(0)=1, a(1)=20, a(2)=380, a(3)=7220, a(4)=137180,
   a(5)=2606420, a(6)=49521790.
   For n ≥ 7:
   a(n) = 18*(a(n‑1)+a(n‑2)+a(n‑3)+a(n‑4)+a(n‑5)) – 171*a(n‑6).   */

#include <stdlib.h>
#include "../utils/mul_small.h"

static seq_t *cache = NULL;          /* memoisation buffer */
static idx_t  cache_capacity = 0;    /* allocated size (elements) */
static idx_t  cache_size = 0;        /* number of computed terms */

/* public function */
seq_t a(idx_t n)
{
    /* initialise cache and base terms once */
    if (cache == NULL)
    {
        cache_capacity = (n + 1 > 7) ? (n + 1) : 7;   /* at least for 0..6 */
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));

        /* base values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)20;
        cache[2] = (seq_t)380;
        cache[3] = (seq_t)7220;
        cache[4] = (seq_t)137180;
        cache[5] = (seq_t)2606420;

        /* a(6) computed from the recurrence with the numerator constant 1 */
        seq_t sum6 = cache[1] + cache[2] + cache[3] + cache[4] + cache[5];
        seq_t t6   = mul_small(sum6, 18);
        t6          = t6 - mul_small(cache[0], 171);
        t6          = t6 + (seq_t)1;          /* +1 from numerator coefficient */
        cache[6] = t6;

        cache_size = 7;
    }

    /* extend cache if not large enough for n */
    if (n >= cache_capacity)
    {
        idx_t new_cap = cache_capacity * 2;
        if (new_cap <= n) new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_capacity = new_cap;
    }

    /* return if already computed */
    if (n < cache_size)
        return cache[n];

    /* compute new terms up to n */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        seq_t term;
        if (i == 6)                /* already known, should not happen */
            term = cache[6];
        else
        {
            /* sum of the previous five terms */
            seq_t s = cache[i-1] + cache[i-2] + cache[i-3]
                    + cache[i-4] + cache[i-5];
            term = mul_small(s, 18);
            term = term - mul_small(cache[i-6], 171);
            /* for i >= 7 the numerator coefficient is 0 */
        }
        cache[i] = term;
    }

    cache_size = n + 1;
    return cache[n];
}
