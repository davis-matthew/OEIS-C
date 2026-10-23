/*
 * A170584  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 47 * 46^(n-1) for 1 ≤ n ≤ 46 Recurrence for n ≥ 48: a(n)
 *            = 45*(a(n-1)+…+a(n-46)) - 1035*a(n-47) a(0) = 1, a(47) =
 *            45*(a(1)+…+a(46)) - 1034
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;           /* number of elements stored (next free index) */
    static seq_t sum_window = 0;          /* sum of last 46 computed elements */
    static seq_t pow46 = 0;               /* holds 47*46^(k-1) when k ≤ 46 */

    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
    }

    /* Grow internal cache if needed */
    if (n + 1 > cache_cap) {
        cache_cap = n + 1;
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * cache_cap);
    }

    /* Extend cache up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {                     /* a(0) = 1 */
            cache[i] = (seq_t)1;
        } else if (i <= 46) {              /* a(i) = 47 * 46^(i-1) */
            if (i == 1) {
                pow46 = (seq_t)47;
            } else {
                pow46 = pow46 * (seq_t)46;
            }
            cache[i] = pow46;
            sum_window += cache[i];
        } else if (i == 47) {
            /* a(47) = 45 * sum_{k=1}^{46} a(k) - 1034 */
            cache[i] = (seq_t)45 * sum_window - (seq_t)1034;
            /* Update sum_window for next iteration (will be sum of a(2)…a(47)) */
            sum_window = sum_window - cache[1] + cache[i];
        } else { /* i ≥ 48 */
            cache[i] = (seq_t)45 * sum_window - (seq_t)1035 * cache[i - 47];
            /* Update sum_window for the next index:
               sum of a(i-46)…a(i) = previous sum_window + a(i) - a(i-46) */
            sum_window += cache[i];
            sum_window -= cache[i - 46];
        }
    }
    cache_len = n + 1;
    return cache[n];
}
