/*
 * A169310  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^30 = I.
 * https://oeis.org/A169310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=9, a(n)=8*a(n-1) for 2≤n<30. For n≥30: a(n) =
 *            -2*a(n-1) + 7*∑_{k=2}^{29} a(n-k) - 28*a(n-30), derived from the
 *            rational generating function
 *            (t^30+2t^29+…+2t+1)/(28t^30-7t^29-…-7t^2+2t+1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static memoisation cache */
    static idx_t cache_len = 0;   /* number of elements stored (max index+1) */
    static seq_t *cache = NULL;   /* pointer to the cache array */

    /* extend or initialise cache up to index n */
    if (n + 1 > cache_len) {
        /* allocate new array large enough */
        seq_t *new_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;           /* out‑of‑memory */

        /* copy existing values, if any */
        if (cache) {
            memcpy(new_cache, cache, cache_len * sizeof(seq_t));
            free(cache);
        }

        /* compute missing values */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val;
            if (i == 0) {
                val = (seq_t)1;
            } else if (i == 1) {
                val = (seq_t)9;
            } else if (i < 30) {               /* a(i)=8*a(i-1) */
                val = new_cache[i - 1] * (seq_t)8;
            } else {                           /* recurrence for i≥30 */
                seq_t sum = (seq_t)0;
                for (idx_t k = 2; k <= 29; ++k) {
                    sum += new_cache[i - k];
                }
                sum *= (seq_t)7;            /* 7 * Σ a(i-k) */
                sum -= new_cache[i - 1] * (seq_t)2; /* -2*a(i-1) */
                sum -= new_cache[i - 30] * (seq_t)28;/* -28*a(i-30)*/
                val = sum;
            }
            new_cache[i] = val;
        }

        /* replace old cache with the new one */
        cache = new_cache;
        cache_len = n + 1;
    }

    return cache[n];
}
