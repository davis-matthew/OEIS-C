/*
 * A170650  Number of reduced words of length n in Coxeter group on 17
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for 1<=n<=48, a(n)=2+15*(a(n-1)+...+a(max(0,n-48))); for
 *            n>=49, a(n)=15*(a(n-1)+...+a(n-48))-120*a(n-49)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) of OEIS sequence A170650 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* array of computed terms */
    static idx_t cache_cap = 0;       /* allocated capacity */
    static idx_t cache_len = 0;       /* number of terms computed */

    /* Grow the cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap == 0 ? 1 : cache_cap;
        while (new_cap < n + 1) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (cache_len <= n) {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = 0;

            /* Sum the previous up to 48 terms */
            for (idx_t k = 1; k <= 48 && k <= i; ++k) {
                sum += cache[i - k];
            }

            if (i == 0) {
                cache[0] = 1;
            } else if (i <= 48) {
                /* a(i) = 2 + 15 * sum */
                cache[i] = 2 + sum * 15;
            } else {
                /* a(i) = 15 * sum - 120 * a(i-49) */
                cache[i] = sum * 15 - cache[i - 49] * 120;
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
