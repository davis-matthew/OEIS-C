/*
 * A074225  a(n) = n * Sum_{d|n} d*2^(d-1).
 * https://oeis.org/A074225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Sum_{d|n} d * 2^(d-1)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 53, u128 n <= 115,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1; 0 is undefined */
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            seq_t term1 = (seq_t)d * pow2(d - 1);
            sum += term1;
            idx_t other = n / d;
            if (other != d) {
                seq_t term2 = (seq_t)other * pow2(other - 1);
                sum += term2;
            }
        }
    }
    return (seq_t)n * sum;
}
