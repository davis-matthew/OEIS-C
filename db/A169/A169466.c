/*
 * A169466  Number of reduced words of length n in Coxeter group on 21
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^33 = I.
 * https://oeis.org/A169466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 21 ... For 0 < n <= 32: a(n) = N(n) + 19 *
 *            Σ_{k=0}^{n-1} a(k), N(0)=1, N(n)=2 for 1<=n<=32 For n >= 33:
 *            a(n) = 19 * Σ_{i=1}^{32} a(n-i) - 190 * a(n-33) The recurrence
 *            derives from the rational generating function given in the OEIS
 *            entry
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   allocate_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "../utils/allocate_seq_array.h"  /* seq_t *allocate_seq_array(size_t) */

static seq_t *cache = NULL;          /* array of computed a(n) values */
static size_t cache_len = 0;         /* current allocated length */

seq_t a(idx_t n)
{
    /* extend cache to hold up to index n */
    if (n >= cache_len)
    {
        size_t old_len = cache_len;
        size_t new_len = (cache_len == 0) ? 1 : cache_len * 2;
        while (new_len <= (size_t)n)
            new_len *= 2;

        /* allocate new cache array */
        seq_t *new_cache = allocate_seq_array(new_len);
        /* copy old values */
        if (old_len > 0)
            memcpy(new_cache, cache, old_len * sizeof(seq_t));
        /* zero-initialize remainder */
        if (new_len > old_len)
            memset(new_cache + old_len, 0, (new_len - old_len) * sizeof(seq_t));

        /* replace old cache */
        cache = new_cache;
        cache_len = new_len;
    }

    /* compute missing values up to n */
    for (idx_t i = 0; i <= n; ++i)
    {
        /* skip already computed values */
        if (i < cache_len && cache[i] != (seq_t)0)
            continue;

        if (i == 0)
        {
            cache[0] = (seq_t)1;
        }
        else if (i <= 32)
        {
            /* compute sum of previous terms */
            seq_t sumPrev = 0;
            for (idx_t k = 0; k < i; ++k)
                sumPrev += cache[k];

            seq_t N = (seq_t)2;              /* N(i) = 2 for 1 <= i <= 32 */
            cache[i] = N + (seq_t)19 * sumPrev;
        }
        else /* i >= 33 */
        {
            /* sum of the last 32 terms: a(i-1) ... a(i-32) */
            seq_t sumLast32 = 0;
            for (idx_t k = 1; k <= 32; ++k)
                sumLast32 += cache[i - k];

            cache[i] = (seq_t)19 * sumLast32
                     - (seq_t)190 * cache[i - 33];
        }
    }

    return cache[n];
}
