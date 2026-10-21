/*
 * A231683  a(n) = Sum_{i=0..n} digsum_8(i)^4, where digsum_8(i) = A053829(i).
 * https://oeis.org/A231683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = \sum_{i=0..n} digsum_8(i)^4 where digsum_8(i) = sum of
 *            the digits of i in base 8 (OEIS A053829)
 * Range:     u8 n <= 3, u16 n <= 36, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   octal_digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/octal_digit_sum.h"

/* Memoization buffer for already computed values of the sequence. */
static seq_t *cache = NULL;   /* sequence values */
static idx_t  cache_len = 0; /* number of stored values: cache_len-1 is max n computed */

seq_t a(idx_t n)
{
    /* Return if already known. */
    if (n < cache_len) {
        return cache[n];
    }

    /* Extend the cache to hold values up to n. */
    idx_t old_len = cache_len;
    if (n + 1 > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!tmp) {
            /* Allocation failure – return 0 as a safe fallback. */
            return 0;
        }
        cache = tmp;
        cache_len = n + 1;
    }

    /* If this is the first call, initialise a(0). */
    if (old_len == 0) {
        cache[0] = 0;
        old_len = 1;
    }

    /* Compute the sequence values from old_len up to n. */
    for (idx_t i = old_len; i <= n; ++i) {
        seq_t d = octal_digit_sum(i);          /* digital sum in base 8 */
        seq_t d2 = d * d;                      /* d^2 */
        seq_t d4 = d2 * d2;                    /* d^4 */
        cache[i] = cache[i - 1] + d4;          /* cumulative sum */
    }

    return cache[n];
}
