/*
 * A056155  Positive integer k, 1 <= k <= n, which maximizes k^(n+1-k).
 * https://oeis.org/A056155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the integer k, 1 <= k <= n, that maximises k^(n+1-k).
 *            The function computes this value by comparing (n+1-k)*log(k) for
 *            each candidate; the maximum yields the required k. An O(n) scan
 *            is performed for each call
 * Range:     u8 n <= 1670, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n < 1, return 0 for safety */
        return 0;

    idx_t best_k = 1;
    double best_val = -1.0;   /* smaller than any attainable value */

    for (idx_t k = 1; k <= n; ++k) {
        double val = (double)(n + 1 - k) * log((double)k);

        /* Update best if val is strictly greater; ties keep the first k */
        if (val > best_val) {
            best_val = val;
            best_k   = k;
        }
    }

    return (seq_t)best_k;
}
