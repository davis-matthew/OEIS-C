/*
 * A080267  a(n) = Sum_{d divides n} d*2^(n-n/d).
 * https://oeis.org/A080267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d divides n} d * 2^(n - n/d)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 27, u64 n <= 59, u128 n <= 121,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Sum of d * 2^(n - n/d) over all positive divisors d of n.
   n is assumed >= 1. The calculation is performed in seq_t and
   uses the utility pow2(idx_t) to obtain 2^(exponent). */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* Iterate over divisor pairs (i, n/i) */
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            seq_t term1 = (seq_t)d1 * pow2(n - n / d1);
            sum += term1;

            idx_t d2 = n / i;
            if (d2 != d1) {
                seq_t term2 = (seq_t)d2 * pow2(n - n / d2);
                sum += term2;
            }
        }
    }
    return sum;
}
