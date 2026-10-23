/*
 * A164693  Number of reduced words of length n in Coxeter group on 48
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Recurrence from the generating function of A164693:
   a(0) = 1
   a(1) = 48
   a(2) = 2256
   a(3) = 106032
   a(4) = 4983504
   a(5) = 234224688
   a(6) = 11008560336
   a(7) = 46*(a(6)+a(5)+a(4)+a(3)+a(2)+a(1)) - 1081*a(0) + 1
   For n ≥ 8:
   a(n) = 46*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6)) - 1081*a(n-7)
*/
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache with the first computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len  = 0;   /* number of stored terms (highest index + 1) */
    static idx_t cache_cap  = 0;   /* allocated capacity */

    /* initialize cache with first seven values and compute a(7) */
    if (cache_len == 0)
    {
        /* allocate enough space for at least 8 terms (0..7) */
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* fallback in case of allocation failure */

        /* first seven terms */
        cache[0] = 1ULL;
        cache[1] = 48ULL;
        cache[2] = 2256ULL;
        cache[3] = 106032ULL;
        cache[4] = 4983504ULL;
        cache[5] = 234224688ULL;
        cache[6] = 11008560336ULL;

        /* compute a(7) with the +1 constant from the numerator */
        {
            seq_t sum = 0;
            for (int k = 1; k <= 6; ++k)
                sum += cache[7 - k];
            cache[7] = 46 * sum - 1081 * cache[0] + 1;
        }
        cache_len = 8;  /* terms 0..7 are ready */
    }

    /* grow cache if needed */
    if (n >= cache_len)
    {
        /* determine new capacity: at least n+1 */
        idx_t new_cap = n + 1;
        if (new_cap > cache_cap)
        {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0; /* fallback */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute missing terms up to n */
        for (idx_t i = cache_len; i <= n; ++i)
        {
            seq_t sum = 0;
            for (int k = 1; k <= 6; ++k)
                sum += cache[i - k];

            seq_t term = 46 * sum - 1081 * cache[i - 7];
            if (i == 7)
                term += 1;      /* special constant for a(7) */
            cache[i] = term;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
