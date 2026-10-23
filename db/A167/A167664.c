/*
 * A167664  Number of reduced words of length n in Coxeter group on 11
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 9*a(n-1)+9*a(n-2)+…+9*a(n-14) − 45*a(n-15) + b(n),
   where b(0)=1, b(1…14)=2, b(15)=1, and b(n)=0 for n>15.  */
#include <stddef.h>
#include "../utils/ensure_cache.h"

/* cache for computed values */
static seq_t *cache = NULL;          /* holds a(0) … a(computed_len-1) */
static size_t cache_cap = 0;         /* allocated capacity (in seq_t elements) */
static idx_t computed_len = 0;       /* number of values currently stored */

/* function to compute a(n) */
seq_t a(idx_t n)
{
    /* allocate or grow cache to at least n+1 elements */
    ensure_cache(n + 1, &cache, &cache_cap);

    /* compute missing values sequentially */
    for (idx_t k = computed_len; k <= n; ++k)
    {
        /* compute b(k) */
        seq_t b = 0;
        if (k == 0)          b = (seq_t)1;
        else if (k <= 14)    b = (seq_t)2;
        else if (k == 15)    b = (seq_t)1;

        /* sum of 9*a(k-i) for i=1..14 if k>=i */
        seq_t sum = 0;
        for (idx_t i = 1; i <= 14; ++i)
        {
            if (k >= i)
                sum = sum + (seq_t)9 * cache[k - i];
        }

        /* subtract 45*a(k-15) if k>=15 */
        seq_t term = sum;
        if (k >= 15)
            term = term - (seq_t)45 * cache[k - 15];

        /* add b(k) */
        term = term + b;

        cache[k] = term;
    }

    computed_len = n + 1;
    return cache[n];
}
