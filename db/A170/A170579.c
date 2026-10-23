/*
 * A170579  Number of reduced words of length n in Coxeter group on 42
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for 1 ≤ n ≤ 46: a(n) = 40*∑_{i=0}^{n-1} a(i) + 2;
 *            a(47) = 1 – 820*a(0) + 40*∑_{i=1}^{46} a(i); for n ≥ 48: a(n) =
 *            40*∑_{i=1}^{46} a(n-i) – 820*a(n-47)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[0..cache_len-1] contains a(0)..a(cache_len-1) */
static idx_t cache_len = 0;     /* number of valid entries in cache            */
static size_t cache_cap = 0;     /* allocated capacity of cache                */

static void grow_cache(idx_t n)
{
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();     /* allocation failed – terminate */
        cache = new_cache;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    grow_cache(n);

    /* compute any missing terms */
    if (cache_len <= n) {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = 0;

            if (i == 0) {                 /* base case */
                cache[0] = (seq_t)1;
                continue;
            }

            /* sum of the appropriate preceding block of terms */
            idx_t start = (i >= 46) ? i - 46 : 0;
            for (idx_t j = start; j < i; ++j)
                sum += cache[j];

            if (i <= 46) { /* 1 ≤ i ≤ 46 */
                cache[i] = (seq_t)40 * sum + (seq_t)2;
            } else if (i == 47) { /* special case for i = 47 */
                seq_t temp = (seq_t)1 + (seq_t)40 * sum;
                temp -= (seq_t)820 * cache[0];
                cache[i] = temp;
            } else { /* i ≥ 48 */
                seq_t res = (seq_t)40 * sum;
                res -= (seq_t)820 * cache[i - 47];
                cache[i] = res;
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
