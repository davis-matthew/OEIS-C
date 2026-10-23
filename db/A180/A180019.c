/*
 * A180019  Difference of sums of digits of n in decimal and in ternary
 *          representation.
 * https://oeis.org/A180019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = sum of decimal digits of n – sum of ternary digits of n
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*
 * Computes A180019: a(n) = A007953(n) – A053735(n)
 * Where A007953(n) = sum of decimal digits of n
 *       A053735(n) = sum of ternary digits of n
 */
extern seq_t a(idx_t n)
{
    seq_t sum_dec = 0;
    seq_t sum_tern = 0;
    idx_t x = n;

    /* Sum decimal digits */
    while (x > 0) {
        sum_dec += (seq_t)(x % 10);
        x /= 10;
    }

    /* Sum ternary digits */
    x = n;
    while (x > 0) {
        sum_tern += (seq_t)(x % 3);
        x /= 3;
    }

    return sum_dec - sum_tern;
}
