/*
 * A170445  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 4·3^(n-1) for 1 ≤ n ≤ 44, a(n) =
 *            (2/3)·∑_{i=1}^{44} a(n-i) for n ≥ 45 (All divisions are exact
 *            integers)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* array of cached values */
static idx_t  cache_len = 0;      /* number of elements currently cached */

seq_t a(idx_t n)
{
    /* Grow the cache if necessary */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;      /* out‑of‑memory fallback */

        cache = new_cache;

        /* Compute new values */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i <= 44) {
                if (i == 1) {
                    cache[1] = (seq_t)4;
                } else {
                    cache[i] = cache[i - 1] * (seq_t)3;
                }
            } else { /* i >= 45 */
                seq_t sum = (seq_t)0;
                /* Sum of the previous 44 terms */
                for (idx_t j = i - 44; j < i; ++j)
                    sum += cache[j];
                /* a(i) = (2/3) * sum */
                cache[i] = (sum * (seq_t)2) / (seq_t)3;
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
