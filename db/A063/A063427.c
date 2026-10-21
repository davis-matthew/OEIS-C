/*
 * A063427  a(n) is the smallest positive integer k such that n*k/(n+k) is an
 *          integer.
 * https://oeis.org/A063427
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer k such that n*k/(n+k) is an
 *            integer. Computed by enumerating all divisors m of n with m < n
 *            and choosing the minimal k = m*n/(n-m) that is integer
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t mink = 0;
    bool first = true;
    for (idx_t m = 1; m < n; ++m) {
        idx_t d = n - m;                     /* divisor of (m*n) */
        seq_t prod = (seq_t)m * (seq_t)n;    /* m*n */
        if ((prod % (seq_t)d) == 0) {
            seq_t k = prod / (seq_t)d;
            if (first || k < mink) {
                mink = k;
                first = false;
            }
        }
    }
    return mink;
}
