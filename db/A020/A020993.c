/*
 * A020993  List of scores that can be achieved with four darts all of which
 *          hit a dartboard with regions labeled 1, 5, 10, 25.
 * https://oeis.org/A020993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 33, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the descending list of distinct sums of four darts,
 * each dart hitting a region valued 1, 5, 10, or 25.
 * Generates the list once on the first call and caches it. */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* cache holds the distinct sums in descending order.
       The maximum possible sum is 4*25 = 100, so the cache size
       need not exceed 101.  The number of distinct sums is 34. */
    static seq_t cache[101];
    static size_t cache_len = 0;

    if (!cache_len) {
        /* Mark which sums are achievable. */
        bool visited[101] = {false};

        for (idx_t d = 0; d <= 4; ++d) {
            for (idx_t c = 0; c <= 4 - d; ++c) {
                for (idx_t b = 0; b <= 4 - d - c; ++b) {
                    idx_t a_val = 4 - d - c - b;   /* number of 1‑point darts */
                    seq_t s = (seq_t)a_val
                           + (seq_t)b * 5
                           + (seq_t)c * 10
                           + (seq_t)d * 25;
                    /* s is guaranteed to be between 0 and 100 */
                    visited[(int)s] = true;
                }
            }
        }

        /* Fill the cache with visited values in descending order. */
        size_t idx = 0;
        for (int val = 100; val >= 0; --val) {
            if (visited[val]) {
                cache[idx++] = (seq_t)val;
            }
        }
        cache_len = idx;  /* expected to be 34 */
    }

    if ((idx_t)n < (idx_t)cache_len)
        return cache[(size_t)n];

    /* Sequence terminates after the last distinct sum. */
    return (seq_t)0;
}
