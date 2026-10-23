/*
 * A170655  Number of reduced words of length n in Coxeter group on 22
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20*sum_{i=1}^{48} a(n-i) - 210*a(n-49) + rhs where rhs =
 *            1 if n==0 or n==49, rhs = 2 if 1 <= n <= 48, rhs = 0 otherwise.
 *            This follows from the rational generating function G(t) = (t^49
 *            + 2*t^48 + ... + 2*t + 1) / (1 - 20*t - 20*t^2 - ... - 20*t^48 +
 *            210*t^49)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* number of values stored (cache_size == highest index + 1) */

    if (n >= cache_size) {
        /* enlarge cache */
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failed – aborting silently (caller assumes success) */
            return (seq_t)0;
        }
        cache = new_cache;

        /* compute new terms */
        for (idx_t k = cache_size; k <= n; ++k) {
            seq_t sum48 = 0;
            for (idx_t i = 1; i <= 48 && k >= i; ++i) {
                sum48 += cache[k - i];
            }
            seq_t prev49 = (k >= 49) ? cache[k - 49] : (seq_t)0;

            seq_t rhs = 0;
            if (k == 0 || k == 49) {
                rhs = 1;
            } else if (k <= 48) { /* 1 <= k <= 48 */
                rhs = 2;
            }

            /* a(k) = 20*sum48 - 210*prev49 + rhs */
            seq_t term = 20 * sum48;
            term = term - 210 * prev49 + rhs;
            cache[k] = term;
        }

        cache_size = new_size;
    }

    return cache[n];
}
