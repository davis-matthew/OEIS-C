/*
 * A119428  G.f.: A(x) = 1 + Sum_{n>=0} (-1)^n* Sum_{k=1..4}
 *          x^(5n+k)/(1-x^(5n+k)).
 * https://oeis.org/A119428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; for n>0, a(n) = sum_{d|n, (n/d) mod 5 ≠ 0}
 *            (-1)^{floor((n/d)/5)}
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {        /* iterate over divisors d */
        if (n % d == 0) {
            /* divisor k = d, quotient t = n / d */
            idx_t t = n / d;
            if (t % 5 != 0) {
                idx_t m = t / 5;
                seq_t sign = (seq_t)(((m & 1) != 0) ? -1 : 1);
                sum += sign;
            }

            /* the complementary divisor k = n / d, quotient t = d */
            idx_t d2 = n / d;
            if (d2 != d) {
                idx_t t2 = d;               /* because n / (n/d) = d */
                if (t2 % 5 != 0) {
                    idx_t m2 = t2 / 5;
                    seq_t sign = (seq_t)(((m2 & 1) != 0) ? -1 : 1);
                    sum += sign;
                }
            }
        }
    }
    return sum;
}
