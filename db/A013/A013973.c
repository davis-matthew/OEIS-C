/*
 * A013973  Expansion of Eisenstein series E_6(q) (alternate convention
 *          E_3(q)).
 * https://oeis.org/A013973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 (n==0) else -504 * sigma_5(n), where sigma_5(n) = sum
 *            of fifth powers of divisors of n
 * Range:     i8 n <= 0, i16 n <= 2, i32 n <= 21, i64 n <= 1775,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            seq_t d5 = (seq_t)d * d * d * d * d;
            sum += d5;
            idx_t other = n / d;
            if (other != d) {
                seq_t o5 = (seq_t)other * other * other * other * other;
                sum += o5;
            }
        }
    }
    return - (seq_t)504 * sum;
}
