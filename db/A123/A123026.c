/*
 * A123026  a(n) = n!*b(n), where b(n) = ((n-1)^2 - 2)*b(n-2)/(n*(n-1)) and
 *          b(0) = b(1) = 1.
 * https://oeis.org/A123026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ((n-1)^2 - 2) * a(n-2) with a(0) = a(1) = 1
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 21, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;

    if ((n & 1) == 0) {                /* even n : start from a(0)=1 */
        seq_t prev = (seq_t)1;          /* a(0) */
        for (idx_t i = 2; i <= n; i += 2) {
            seq_t coef = (seq_t)(i - 1) * (seq_t)(i - 1) - 2;
            result = coef * prev;
            prev = result;
        }
    } else {                           /* odd n : start from a(1)=1 */
        seq_t prev = (seq_t)1;          /* a(1) */
        for (idx_t i = 3; i <= n; i += 2) {
            seq_t coef = (seq_t)(i - 1) * (seq_t)(i - 1) - 2;
            result = coef * prev;
            prev = result;
        }
    }

    return result;
}
