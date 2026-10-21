/*
 * A214630  a(n) is the reduced numerator of 1/4 - 1/A109043(n)^2 = (1 -
 *          1/A026741(n)^2)/4.
 * https://oeis.org/A214630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac, easy
 * Formula:   a(4*k) = 4*k^2 - 1 a(2*k+1) = k*(k+1) (covers all odd indices)
 *            a(4*k+2) = k*(k+1) (covers even indices not divisible by 4)
 * Range:     i8 n <= 22, i16 n <= 362, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    // handle odd indices: n = 2*m + 1
    if ((n & 1) == 1) {
        seq_t m = ((seq_t)(n - 1)) / 2;
        return m * (m + 1);
    }

    // even indices
    if ((n & 3) == 0) {                 // n % 4 == 0
        seq_t k = (seq_t)(n / 4);
        return (seq_t)4 * k * k - 1;
    }

    // n % 4 == 2
    seq_t k = (seq_t)(n / 4);
    return k * (k + 1);
}
