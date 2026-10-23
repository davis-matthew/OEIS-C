/*
 * A157019  a(n) = Sum_{d|n} binomial(n/d+d-2, d-1).
 * https://oeis.org/A157019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{d|n} binomial(n/d + d - 2, d - 1)
 * Range:     u8 n <= 24, u16 n <= 89, u32 n <= 318, u64 n <= 1169,
 *            u128 n <= 4409, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0) continue;

        idx_t a1 = n / d + d - 2;
        idx_t b1 = d - 1;
        sum += binom(a1, b1);

        idx_t d2 = n / d;
        if (d2 != d) {
            idx_t a2 = d + d2 - 2;   /* equivalent to n/d2 + d2 - 2 */
            idx_t b2 = d2 - 1;
            sum += binom(a2, b2);
        }
    }
    return sum;
}
