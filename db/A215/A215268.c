/*
 * A215268  Concatenation of the decimal digits of n^2-1 and n^2.
 * https://oeis.org/A215268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, less, easy
 * Formula:   a(n) = concatenate((n^2 - 1), n^2)
 * Range:     i8 n <= 3, i16 n <= 9, i32 n <= 146, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   digit_count, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow10.h"     /* seq_t pow10(idx_t e); */
#include "../utils/digit_count.h"  /* uint64_t digit_count(uint64_t n); */

extern seq_t a(idx_t n)
{
    /* Special case for the offset zero */
    if (n == 0) {
        /* n^2 = 0, n^2-1 = -1 → “-1” followed by “0” = -10 */
        return (seq_t)-10;
    }

    /* Compute n^2 in the appropriate type */
    seq_t sq = (seq_t)n * (seq_t)n;

    /* Digit count of n^2 – use the helper on a uint64_t since
       n^2 fits into 64 bits for the input ranges expected. */
    uint64_t sq_ll = (uint64_t)sq;
    idx_t d = (idx_t)digit_count(sq_ll);

    /* 10^d as seq_t */
    seq_t pow10_val = pow10(d);

    /* Concatenate: (n^2-1)·10^d + n^2 */
    return (sq - 1) * pow10_val + sq;
}
