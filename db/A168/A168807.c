/*
 * A168807  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^19 = I.
 * https://oeis.org/A168807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 32*(a(n‑1)+…+a(n‑18)) – 528*a(n‑19) + p(n) where p(0)=1,
 *            p(1)…p(18)=2, p(19)=1, p(n)=0 for n≥20 (derived from G(t) =
 *            (t^19+2t^18+…+2t+1)/(528t^19-32t^18-…-32t+1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   alloc_seq_array, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* memory for computed terms          */
    static size_t cache_cap = 0;    /* total allocated capacity            */
    static size_t cache_len = 0;    /* number of terms stored (last index +1) */

    /* Initialise the cache on the first call               */
    if (cache == NULL)
    {
        /* Allocate a modest initial block; grow as needed   */
        cache_cap = 64;
        cache     = alloc_seq_array(cache_cap);
        /* a(0) = 1 */
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Ensure the cache is large enough for n */
    if ((size_t)n + 1 > cache_cap)
        grow_seq(&cache, &cache_cap, (size_t)n + 1);

    /* Compute any missing terms up to n */
    if ((idx_t)cache_len <= n)
    {
        for (size_t i = cache_len; i <= (size_t)n; ++i)
        {
            /* Determine the p(i) contribution from the numerator   */
            seq_t pi;
            if (i == 0)
                pi = (seq_t)1;
            else if (i <= 18)
                pi = (seq_t)2;
            else if (i == 19)
                pi = (seq_t)1;
            else
                pi = (seq_t)0;

            /* Sum of the previous 18 terms (if present)             */
            seq_t sum = (seq_t)0;
            for (size_t j = 1; j <= 18 && i >= j; ++j)
                sum += cache[i - j];

            /* Compute the recurrence: 32*sum + pi – 528*a(i‑19) (when applicable) */
            seq_t val = sum * (seq_t)32 + pi;
            if (i >= 19)
                val -= cache[i - 19] * (seq_t)528;

            cache[i] = val;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
