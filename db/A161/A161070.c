/*
 * A161070  Number of partitions of n into powers of two minus one where every
 *          part appears at least 8 times.
 * https://oeis.org/A161070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 2^k-1 (k≥1) such
 *            that each distinct part used appears at least 8 times
 * Range:     u8 n <= 169, u16 n <= 615, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Offset starts at 1; for n < 1 return 0. */
    if (n < 1) return (seq_t)0;

    /* Allocate dynamic arrays for dynamic programming. */
    size_t size = (size_t)n + 1;
    seq_t *dp_cur  = (seq_t *)calloc(size, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(size, sizeof(seq_t));
    if (!dp_cur || !dp_next) {
        free(dp_cur);
        free(dp_next);
        return (seq_t)0;
    }

    dp_cur[0] = (seq_t)1;   /* empty partition */

    /* Iterate over part sizes p = 2^k - 1. */
    for (uint64_t p = 1; p <= n; p = p * 2 + 1) {
        for (uint64_t r = 0; r < p; ++r) {
            seq_t cum      = (seq_t)0;    /* cumulative sum of dp_cur values seen so far */
            seq_t last7    = (seq_t)0;    /* sum of the last 7 dp_cur values */
            seq_t buf[7]   = {(seq_t)0}; /* circular buffer for last 7 values */
            size_t buf_idx = 0;

            for (uint64_t idx = r, i = 0; idx <= n; idx += p, ++i) {
                seq_t add = (seq_t)0;
                if (i >= 8) {
                    add = cum - last7;   /* sum of dp_cur up to index i-8 */
                }
                dp_next[idx] = dp_cur[idx] + add;

                /* update running sums and buffer */
                cum += dp_cur[idx];
                last7 += dp_cur[idx];
                last7 -= buf[buf_idx];
                buf[buf_idx] = dp_cur[idx];
                buf_idx = (buf_idx + 1) % 7;
            }
        }
        /* Prepare for next part: swap dp_cur and dp_next. */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
        /* dp_next will be overwritten entirely in the next iteration */
    }

    seq_t result = dp_cur[n];
    free(dp_cur);
    free(dp_next);
    return result;
}
