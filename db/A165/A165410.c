/*
 * A165410  Hankel transform of the transform of 2^n given by A165409.
 * https://oeis.org/A165410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(3k) = (-1)^k * 2^(3k^2 + k) a(3k+1) = 0 a(3k+2) = (-1)^(k+1) *
 *            2^(3k^2 + 5k + 2)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 10, i64 n <= 13, i128 n <= 18,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* zeros at indices 1,4,7,... */
    if ((n % 3) == 1) {
        return 0;
    }

    idx_t k;
    idx_t exp;
    if ((n % 3) == 0) {          /* n = 3*k */
        k   = n / 3;
        exp = 3 * k * k + k;
    } else {                     /* n = 3*k + 2 */
        k   = (n - 2) / 3;
        exp = 3 * k * k + 5 * k + 2;
    }

    seq_t val = pow2(exp);       /* compute 2^exp */

    /* Apply the sign (-1)^floor((n+2)/3) */
    if (((n + 2) / 3) & 1ULL) {
        val = -val;
    }

    return val;
}
