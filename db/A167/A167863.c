/*
 * A167863  Number of reduced words of length n in Coxeter group on 48
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0)=1.
   For k>0,
       a(k) = 46*(a(k-1)+a(k-2)+...+a(k-14)) - 1081*a(k-15) + c_k
   where   c_k = 1  if k==0 or k==15
            c_k = 2  if 1 <= k <= 14
            c_k = 0  if k > 15
   The sequence satisfies the rational generating function
   (t^15+2t^14+⋯+2t+1)/(1081t^15-46t^14-⋯-46t+1).  */
#include <stdlib.h>

static seq_t *cache = NULL;   /* array of computed values */
static size_t cache_len = 0;  /* number of values actually computed */
static size_t cache_cap = 0;  /* allocated capacity */

static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= needed) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Ensure cache capacity up to n */
    grow_cache(n + 1);

    /* Compute missing terms */
    for (idx_t k = cache_len; k <= n; ++k)
    {
        seq_t ck = 0;
        if (k == 0 || k == 15) ck = 1;
        else if (k < 15) ck = 2;

        seq_t sum = 0;
        for (int i = 1; i <= 14; ++i)
            if (k >= (idx_t)i)
                sum += (seq_t)46 * cache[k - i];

        if (k >= 15)
            sum -= (seq_t)1081 * cache[k - 15];

        cache[k] = sum + ck;
    }

    cache_len = n + 1;
    return cache[n];
}
