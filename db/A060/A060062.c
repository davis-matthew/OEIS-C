/*
 * A060062  Fifth column of triangle A060058.
 * https://oeis.org/A060062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1385*C(n+12,12) + 32516*C(n+11,12) + 114318*C(n+10,12) +
 *            87156*C(n+9,12) + 11025*C(n+8,12)
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 71, u128 n <= 1000,
 *            big any n
 * Helpers:   binom12
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binom12.h"

/* Main entry point for sequence A060062. */
seq_t a(idx_t n)
{
    /* Coefficients taken from the numerator of the generating function. */
    static const seq_t coeff[5] = {
        (seq_t)1385,
        (seq_t)32516,
        (seq_t)114318,
        (seq_t)87156,
        (seq_t)11025
    };

    seq_t res = 0;
    for (int k = 0; k < 5; ++k) {
        seq_t b = binom12(n + 12 - k);
        res += coeff[k] * b;
    }
    return res;
}
