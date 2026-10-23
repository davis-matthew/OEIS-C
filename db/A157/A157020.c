/*
 * A157020  a(n) = Sum_{d|n} d*binomial(n/d+d-2,d-1).
 * https://oeis.org/A157020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   a(n) = Sum_{d|n} d * binomial(n/d + d - 2, d - 1)
 * Range:     u8 n <= 17, u16 n <= 59, u32 n <= 239, u64 n <= 989,
 *            u128 n <= 4025, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t m1 = n / d1;
            sum += (seq_t)d1 * binomial(m1 + d1 - 2, d1 - 1);

            idx_t d2 = n / i;
            if (d2 != d1) {
                idx_t m2 = n / d2;   /* equals i */
                sum += (seq_t)d2 * binomial(m2 + d2 - 2, d2 - 1);
            }
        }
    }
    return sum;
}
