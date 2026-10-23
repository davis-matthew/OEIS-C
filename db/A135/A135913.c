/*
 * A135913  a(n) = 2+4*2^n-3^n.
 * https://oeis.org/A135913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2 + 4*2^n - 3^n
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 19, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* compute 2^n using exponentiation by squaring */
    seq_t pow2 = 1;
    seq_t base2 = 2;
    idx_t exp = n;
    while (exp) {
        if (exp & 1) pow2 *= base2;
        base2 *= base2;
        exp >>= 1;
    }

    /* compute 3^n using exponentiation by squaring */
    seq_t pow3 = 1;
    seq_t base3 = 3;
    exp = n;
    while (exp) {
        if (exp & 1) pow3 *= base3;
        base3 *= base3;
        exp >>= 1;
    }

    return (seq_t)2 + (seq_t)4 * pow2 - pow3;
}
