/*
 * A169195  Number of reduced words of length n in Coxeter group on 38
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for 1≤n≤26: a(n)=2+36*∑_{k=0}^{n-1}a(k); for n≥27:
 *            a(n)=36*∑_{j=1}^{26}a(n-j) – 666*a(n-27)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation array */
    static seq_t *cache   = NULL;
    static idx_t cache_len = 0;      /* number of terms stored */
    static idx_t cap      = 0;      /* allocated capacity */

    /* grow the cache if it cannot hold index n */
    if (n + 1 > cap) {
        idx_t new_cap = cap ? (cap << 1) : 8;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cap = new_cap;
    }

    /* sum of all already stored terms (needed for n<27) */
    seq_t running_sum = 0;
    for (idx_t i = 0; i < cache_len; ++i)
        running_sum += cache[i];

    /* sliding window sum of the last 26 terms (used for n≥27) */
    seq_t sum_last_26 = 0;

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            running_sum += cache[0];
        } else if (i <= 26) {
            /* a(i) = 2 + 36 * sum_{k=0}^{i-1} a(k) = 2 + 36 * running_sum */
            cache[i] = (seq_t)2 + (seq_t)36 * running_sum;
            running_sum += cache[i];
        } else { /* i >= 27 */
            if (i == 27) {
                /* initialise the first 26‑term window: a(0)..a(25) */
                for (idx_t j = 0; j <= 25; ++j)
                    sum_last_26 += cache[j];
            }
            /* a(i) = 36*sum_last_26 – 666*a(i-27) */
            seq_t val = (seq_t)36 * sum_last_26 - (seq_t)666 * cache[i - 27];
            cache[i] = val;
            /* update the sliding window for the next iteration */
            sum_last_26 += cache[i] - cache[i - 26];
        }
    }

    cache_len = n + 1;
    return cache[n];
}
