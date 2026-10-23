/*
 * A013841  a(n) = 6^(5*n + 4).
 * https://oeis.org/A013841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^(5*n + 4)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t exp = 5 * n + 4;   /* exponent for the power */
    seq_t base = 6;          /* base of the power */
    seq_t result = 1;        /* accumulator */

    /* fast binary exponentiation */
    while (exp > 0) {
        if (exp & 1) result = result * base;   /* multiply by base when LSB is 1 */
        base = base * base;                    /* square the base */
        exp >>= 1;                             /* shift exponent right */
    }
    return result;
}
