/*
 * A166327  Number of reduced words of length n in Coxeter group on 3
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^11 = I.
 * https://oeis.org/A166327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A166327: Number of reduced words of length n in Coxeter group with
   (S_i)^2 = (S_i S_j)^11 = I.  
   Recurrence: a(0)=1; for 1 <= n <=10, a(n)=3*2^(n-1);
   for n>=11, a(n)=2*a(n-1)-a(n-2)+2*a(n-3)-a(n-4)+2*a(n-5)-a(n-6)+
                 2*a(n-7)-a(n-8)+2*a(n-9)-a(n-10)   (uses 10 previous terms)   */

#include <stdlib.h>

static seq_t *cache = NULL;      /* storage for computed terms */
static idx_t  cache_size = 0;    /* number of terms currently stored */

seq_t a(idx_t n)
{
    /* grow the cache if necessary */
    if (n + 1 > cache_size)
    {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;   /* allocation failure: return 0 as safe fallback */

        /* compute the missing terms */
        for (idx_t i = cache_size; i <= n; ++i)
        {
            seq_t val;
            if (i == 0)
                val = (seq_t)1;
            else if (i <= 10)
            {
                /* a(i) = 3 * 2^(i-1)  (loop to avoid UB for small types) */
                val = (seq_t)3;
                for (idx_t k = 2; k <= i; ++k)
                    val *= (seq_t)2;
            }
            else
            {
                /* recurrence for n >= 11 */
                seq_t pos = (seq_t)2 * new_cache[i-1];
                pos += (seq_t)2 * new_cache[i-3];
                pos += (seq_t)2 * new_cache[i-5];
                pos += (seq_t)2 * new_cache[i-7];
                pos += (seq_t)2 * new_cache[i-9];

                seq_t neg = new_cache[i-2] + new_cache[i-4]
                          + new_cache[i-6] + new_cache[i-8]
                          + new_cache[i-10];

                val = pos - neg;
            }
            new_cache[i] = val;
        }

        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
