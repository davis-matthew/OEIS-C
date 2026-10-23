/*
 * A022906  a(n) = M(n) - m(n) for n >= 2, where M(n) = max{ a(i) + a(n-i): i
 *          = 1..n-1 }, m(n) = min{ a(i) + a(n-i): i = 1..n-1 }.
 * https://oeis.org/A022906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{1≤i<n} (a(i)+a(n-i)) - min_{1≤i<n} (a(i)+a(n-i))
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: a(1)=1.  The function memoizes results for arbitrary call order. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache storing computed values (1‑based indexing) */
    static seq_t *cache = NULL;
    static idx_t max_n = 0;

    /* Initialise cache for n = 1 */
    if (cache == NULL) {
        /* Allocate space for indices 0..1 (index 0 unused) */
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) abort();
        cache[1] = (seq_t)1;
        max_n = 1;
    }

    /* Compute values up to requested n if needed */
    if (n > max_n) {
        idx_t new_size = n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!cache) abort();
        for (idx_t k = max_n + 1; k <= n; ++k) {
            seq_t sum;
            seq_t minsum, maxsum;
            /* first i = 1 pair gives initial min/max */
            sum = cache[1] + cache[k - 1];
            minsum = maxsum = sum;
            /* remaining i from 2 to k-1 */
            for (idx_t i = 2; i < k; ++i) {
                sum = cache[i] + cache[k - i];
                if (sum < minsum) minsum = sum;
                if (sum > maxsum) maxsum = sum;
            }
            cache[k] = maxsum - minsum;
        }
        max_n = n;
    }

    return cache[n];
}
