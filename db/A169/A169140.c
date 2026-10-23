/*
 * A169140  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^26 = I.
 * https://oeis.org/A169140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 29 * (a(n-1)+a(n-2)+...+a(n-25)) 
 *          - 435 * a(n-26) + p_n,
 * where p_0 = 1, p_1 … p_25 = 2, p_26 = 1, and p_n = 0 for n > 26.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_alloc = 0;

seq_t a(idx_t n)
{
    /* enlarge cache if necessary */
    if (n >= cache_alloc)
    {
        size_t needed = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!new_cache)               /* allocation failure */
            return 0;                /* caller will not handle error */
        cache = new_cache;
        /* compute newly required terms */
        for (idx_t i = cache_alloc; i <= n; ++i)
        {
            seq_t sum = 0;
            for (idx_t k = 1; k <= 25; ++k)
                if (i >= k)
                    sum += cache[i - k];

            seq_t val = 29 * sum;
            if (i >= 26)
                val -= 435 * cache[i - 26];

            /* add the constant term p_i */
            if (i == 0)
                val += 1;
            else if (i <= 25)
                val += 2;
            else if (i == 26)
                val += 1;
            /* else p_i = 0, nothing to add */

            cache[i] = val;
        }
        cache_alloc = needed;
    }
    return cache[n];
}
