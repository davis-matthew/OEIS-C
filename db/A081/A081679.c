/*
 * A081679  a(n) = (6^n - 5^n - 4^n - 3^n + 4*2^n)/2.
 * https://oeis.org/A081679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (6^n - 5^n - 4^n - 3^n + 4*2^n)/2 Recurrence for n > 4:
 *            a(n) = 20*a(n-1) - 155*a(n-2) + 580*a(n-3) - 1044*a(n-4) +
 *            720*a(n-5) Initial values: a(0) = 1 , a(1) = 1 , a(2) = 1 , a(3)
 *            = 16 , a(4) = 199 The function caches previously computed terms
 *            for efficient access
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cached_len = 0;
    size_t old_len = cached_len;                  /* length before possible growth */

    /* Grow cache if needed */
    if ((size_t)n >= cached_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {                               /* out of memory – recompute without caching */
            seq_t t0 = 1, t1 = 1, t2 = 1, t3 = 16, t4 = 199;
            if (n == 0) return 1;
            if (n == 1) return 1;
            if (n == 2) return 1;
            if (n == 3) return 16;
            if (n == 4) return 199;
            for (idx_t i = 5; i <= n; ++i) {
                seq_t cur = (seq_t)20 * t0 - (seq_t)155 * t1 + (seq_t)580 * t2
                            - (seq_t)1044 * t3 + (seq_t)720 * t4;
                t0 = t1; t1 = t2; t2 = t3; t3 = t4; t4 = cur;
            }
            return t4;
        }
        cache = tmp;
        cached_len = new_len;
    }

    /* Compute missing values up to n */
    for (size_t i = old_len; i <= (size_t)n; ++i) {
        if (i == 0 || i == 1 || i == 2) {
            cache[i] = 1;
        } else if (i == 3) {
            cache[i] = 16;
        } else if (i == 4) {
            cache[i] = 199;
        } else {
            seq_t t0 = cache[i - 1];
            seq_t t1 = cache[i - 2];
            seq_t t2 = cache[i - 3];
            seq_t t3 = cache[i - 4];
            seq_t t4 = cache[i - 5];
            cache[i] = (seq_t)20 * t0 - (seq_t)155 * t1 + (seq_t)580 * t2
                       - (seq_t)1044 * t3 + (seq_t)720 * t4;
        }
    }

    return cache[(size_t)n];
}
