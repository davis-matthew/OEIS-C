/*
 * A170137  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 30 * (a(n-1)+a(n-2)+…+a(n-37)) - 465 * a(n-38) + p(n)
   where p(n) = 1 for n = 0 or 38,
             p(n) = 2 for 1 ≤ n ≤ 37,
             p(n) = 0 otherwise.  */
#include <stdlib.h>

/* Cache for computed values */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Ensure that cache contains values up to n */
static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;

    /* Resize cache */
    idx_t old_len = cache_len;
    cache_len = n + 1;
    cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));

    for (idx_t i = old_len; i <= n; ++i) {
        seq_t sum = 0;

        /* Sum of 30 * a(i-k) for k = 1 .. 37 */
        for (idx_t k = 1; k <= 37 && i >= k; ++k) {
            sum += (seq_t)30 * cache[i - k];
        }

        /* Subtract 465 * a(i-38) if applicable */
        if (i >= 38) {
            sum -= (seq_t)465 * cache[i - 38];
        }

        /* Add the numerator contribution p(i) */
        seq_t p = 0;
        if (i == 0) {
            p = 1;
        } else if (i <= 37) {
            p = 2;
        } else if (i == 38) {
            p = 1;
        }
        cache[i] = sum + p;
    }
}

/* Public interface */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
