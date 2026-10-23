/*
 * A170700  Number of reduced words of length n in Coxeter group on 19
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^50 = I.
 * https://oeis.org/A170700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A170700
 *
 * a(0) = 1
 * for 1 ≤ n < 50   : a(n) = 2 + 17 * Σ_{k=0}^{n-1} a(k)
 * for n = 50       : a(50) = 17 * Σ_{k=1}^{49} a(k) - 153 * a(0) + 1
 * for n > 50       : a(n) = 17 * Σ_{k=n-49}^{n-1} a(k) - 153 * a(n-50)
 *
 * The sums are maintained by a sliding window of the last 49 values
 * (for n ≥ 50) and by a running total of all previous values (for n < 50).
 * The sequence values are kept in a cached array so that calls with
 * arbitrary n in any order run in O(n) time overall.
 */

#include <stdlib.h>

/* static cache for previously computed terms */
static seq_t *cache = NULL;    /* cache[n] holds a(n) */
static idx_t cache_len = 0;    /* number of cached entries */

// sum of all cached a(k) for k < current index
static seq_t sum_all = 0;

// sliding window sum of the last 49 values, valid when cache_len ≥ 50
static seq_t wsum = 0;

seq_t a(idx_t n)
{
    /* if already computed, just return it */
    if (n < cache_len)
        return cache[n];

    /* extend cache to hold values up to index n */
    idx_t old_len = cache_len;
    cache_len = n + 1;

    seq_t *new_cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
    if (!new_cache)
        return (seq_t)0;           /* allocation failed – return 0 */
    cache = new_cache;

    /* compute new terms consecutively */
    for (idx_t i = old_len; i <= n; ++i) {
        seq_t val;

        if (i == 0) {
            val = (seq_t)1;                    /* a(0) */
        } else if (i < 50) {                   /* 1 ≤ i < 50 */
            val = (seq_t)2 + (seq_t)17 * sum_all;
        } else if (i == 50) {                  /* i = 50 */
            /* wsum currently holds Σ_{k=1}^{49} a(k) */
            val = (seq_t)17 * wsum
                - (seq_t)153 * cache[0]
                + (seq_t)1;
        } else {                               /* i > 50 */
            /* wsum holds Σ_{k=i-49}^{i-1} a(k) */
            val = (seq_t)17 * wsum
                - (seq_t)153 * cache[i - 50];
        }

        cache[i] = val;
        sum_all += val;

        /* update sliding window for next iteration */
        if (i < 49) {
            /* window not yet full */
        } else if (i == 49) {
            /* after computing a[49], window sum = Σ_{k=1}^{49} a(k) */
            wsum = sum_all - cache[0];
        } else {
            /* slide window: drop a[i-49], add a[i] */
            wsum += val - cache[i - 49];
        }
    }

    return cache[n];
}
