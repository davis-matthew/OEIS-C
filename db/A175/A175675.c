/*
 * A175675  a(n) = smallest integer m>=n such that sum of digits of n and m is
 *          >=10.
 * https://oeis.org/A175675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = smallest integer m >= n such that digit_sum(n) + digit_sum(m) >= 10 */
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;           /* n in seq_t */
    seq_t sum_n = digit_sum(sn);   /* digit sum of n */

    for (idx_t m = n; ; ++m) {
        seq_t sm = digit_sum((seq_t)m);   /* digit sum of current m */
        if (sum_n + sm >= (seq_t)10) {
            return (seq_t)m;               /* smallest m satisfying the condition */
        }
    }
}
