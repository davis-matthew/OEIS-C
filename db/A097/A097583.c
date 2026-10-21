/*
 * A097583  Octal representation of the concatenation of the first n decimal
 *          numbers with the most significant digits first.
 * https://oeis.org/A097583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A007094(A007908(n))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the octal representation of the
    decimal concatenation of the integers 1..n  */

#include <stdbool.h>
#include "../utils/digits.h"

seq_t a(idx_t n)
{
    /* Precompute powers of ten up to 10^5 */
    static seq_t pow10[6];
    static bool init = false;
    if (!init) {
        pow10[0] = (seq_t)1;
        for (idx_t d = 1; d <= 5; ++d)
            pow10[d] = pow10[d - 1] * (seq_t)10;
        init = true;
    }

    seq_t val = (seq_t)0;          /* decimal concatenation of 1..n      */
    for (idx_t i = 1; i <= n; ++i) {
        int d = digits(i);          /* number of decimal digits of i     */
        /* d is at most 5 for i <= 10000, so pow10[d] exists       */
        val = val * pow10[d] + (seq_t)i;
    }

    seq_t oct      = (seq_t)0;      /* octal representation (as a decimal number) */
    seq_t mult     = (seq_t)1;      /* multiplier for assembling octal digits in decimal form */
    seq_t zero     = (seq_t)0;

    while (val > zero) {
        seq_t rem = val % (seq_t)8; /* least significant octal digit */
        oct = oct + rem * mult;     /* append digit (as decimal digit)   */
        mult = mult * (seq_t)10;    /* prepare multiplier for next digit */
        val = val / (seq_t)8;       /* shift to next octal digit         */
    }

    return oct;
}
