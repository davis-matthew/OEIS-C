/*
 * A236129  Number of partitions of n that have constant reversal sums; see
 *          Comments.
 * https://oeis.org/A236129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{d | 2n} C(floor(n/d)+floor(d/2)-1, floor(d/2))
 * Range:     u8 n <= 39, u16 n <= 159, u32 n <= 588, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0) return sum;          /* not defined for n < 1 but safe to return 0 */

    idx_t m = n + n;                  /* 2*n */
    idx_t d;

    for (d = 1; d <= m / d; ++d) {    /* iterate over divisors up to sqrt(m) */
        if (m % d != 0) continue;

        /* divisor d */
        idx_t hr  = d / 2;                    /* floor(d/2)         */
        idx_t num = n / d + hr - 1;           /* floor(n/d)+hr-1    */
        if (num >= hr)
            sum += binomial(num, hr);

        /* paired divisor m/d, if distinct */
        idx_t d2 = m / d;
        if (d2 != d) {
            idx_t hr2  = d2 / 2;
            idx_t num2 = n / d2 + hr2 - 1;
            if (num2 >= hr2)
                sum += binomial(num2, hr2);
        }
    }

    return sum;
}
