/*
 * A152864  Deficiency of n, plus the number of proper divisors of n: a(n) =
 *          2n - sigma(n) + d(n) - 1.
 * https://oeis.org/A152864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*n - sigma(n) + d(n) - 1; sigma = sum of all divisors,
 *            d(n) = number of divisors (including n)
 * Range:     i8 n <= 130, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t sigma = 0;   /* sum of all divisors of n               */
    seq_t tau   = 0;   /* total number of divisors of n          */

    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t j = n / i;
            if (i == j) {
                sigma += (seq_t)i;
                tau   += 1;
            } else {
                sigma += (seq_t)i + (seq_t)j;
                tau   += 2;
            }
        }
    }

    seq_t two_n = (seq_t)n + (seq_t)n;
    return two_n - sigma + tau - 1;
}
