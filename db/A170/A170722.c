/*
 * A170722  Number of reduced words of length n in Coxeter group on 41
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^50 = I.
 * https://oeis.org/A170722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 1 * For 1 ≤ n ≤ 49: a(n) = 41 × 40^(n-1) * For n ≥ 50:
 *            a(n) = 39 × (a(n-1)+…+a(n-49)) – 780 × a(n-50) + (n==50 ? 1 : 0)
 *            * * The implementation memoises the sequence in a static array
 *            and evaluates * the recurrence using a sliding window of the
 *            previous 49 terms. This * works for any seq_...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;  /* computed terms */
    static idx_t  cache_len  = 0;     /* number of elements stored */
    static size_t cache_cap  = 0;     /* current allocation size */
    static seq_t window_sum = 0;     /* sum of the last 49 terms */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* generate terms up to index n, memoising them */
    while (cache_len <= n) {
        /* grow the cache if necessary */
        if (cache_len >= cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        seq_t val;
        if (cache_len == 0) {                     /* a(0) = 1 */
            val = (seq_t)1;
        } else if (cache_len <= 49) {              /* 1 ≤ n ≤ 49 */
            if (cache_len == 1) {
                val = (seq_t)41;
            } else {
                /* a(n) = a(n‑1) × 40 */
                val = cache[cache_len - 1] * (seq_t)40;
            }
        } else { /* cache_len ≥ 50 */
            idx_t i = cache_len;                  /* current index */
            /* recurrence: 39×sum(last 49) – 780×a(n‑50), +1 for n==50 */
            val = (seq_t)39 * window_sum
                - (seq_t)780 * cache[i - 50];
            if (i == 50)
                val += (seq_t)1;
        }

        /* store the computed term */
        cache[cache_len++] = val;

        /* update sliding window for the next term */
        if (cache_len >= 2) {                     /* window valid from n≥1 */
            window_sum += val;
            if (cache_len > 50)                   /* drop oldest term */
                window_sum -= cache[cache_len - 50];
        }
    }

    return cache[n];
}
