/*
 * A034729  a(n) = Sum_{ k, k|n } 2^(k-1).
 * https://oeis.org/A034729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k|n} 2^(k-1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t i;

    /* iterate over divisors up to sqrt(n) */
    for (i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* divisor i */
            sum += pow2(i - 1);

            /* paired divisor n/i, avoid double counting for perfect squares */
            idx_t j = n / i;
            if (j != i) {
                sum += pow2(j - 1);
            }
        }
    }

    return sum;
}
