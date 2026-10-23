/*
 * A170574  Number of reduced words of length n in Coxeter group on 37
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A170574: Number of reduced words of length n in the
 * Coxeter group on 37 generators with relations (S_i)^2 = (S_i S_j)^47 = I.
 *
 * The sequence satisfies the rational recurrence
 *   a(0) = 1
 *   a(n) = 35·∑_{k=1}^n a(n-k) + 2            (1 ≤ n ≤ 46)
 *   a(47) = 35·∑_{k=1}^{46} a(47-k) – 629
 *   a(n) = 35·∑_{k=1}^{46} a(n-k) – 630·a(n-47)   (n ≥ 48)
 *
 * This implementation memoises the values in a static array and
 * fills the array up to the requested index on demand.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored (cache_len == 0 ⇒ empty) */

    /* Grow the cache if the requested index is beyond the current range */
    if (n >= cache_len)
    {
        idx_t new_len = n + 1;   /* need indices 0 .. n inclusive */

        /* realloc must be cast to seq_t* for arbitrary-precision builds */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        /* realloc may return NULL only if size is zero; size is non‑zero here. */
        cache = tmp;

        /* Fill the newly allocated part */
        for (idx_t i = cache_len; i < new_len; ++i)
        {
            if (i == 0)
            {
                cache[0] = (seq_t)1;
            }
            else if (i <= 46)
            {
                /* a(i) = 35 * (a(0)+…+a(i-1)) + 2 */
                seq_t sum = (seq_t)0;
                for (idx_t j = 0; j < i; ++j)
                    sum += cache[j];
                cache[i] = (seq_t)35 * sum + (seq_t)2;
            }
            else if (i == 47)
            {
                /* a(47) = 35 * Σ_{k=1}^{46} a(47-k) – 629 */
                seq_t sum = (seq_t)0;
                for (idx_t j = 1; j <= 46; ++j)
                    sum += cache[47 - j];
                cache[47] = (seq_t)35 * sum - (seq_t)629;
            }
            else /* i >= 48 */
            {
                /* a(i) = 35 * Σ_{k=1}^{46} a(i-k) – 630 * a(i-47) */
                seq_t sum = (seq_t)0;
                for (idx_t j = 1; j <= 46; ++j)
                    sum += cache[i - j];
                cache[i] = (seq_t)35 * sum - (seq_t)630 * cache[i - 47];
            }
        }

        /* Update cache length to the new size */
        cache_len = new_len;
    }

    return cache[n];
}
