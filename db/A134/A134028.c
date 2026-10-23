/*
 * A134028  Reversal of n in balanced ternary representation.
 * https://oeis.org/A134028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look, base
 * Formula:   a(n) is n written in balanced ternary, then the digits are
 *            reversed and the new number is evaluated in balanced ternary.
 *            Example: 20 = + - + - (balanced ternary) → a(20) = - + - + = -20
 * Range:     i8 n <= 121, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Convert n to balanced ternary digits (least‑significant digit first). */
    idx_t m = n;              /* unsigned copy for manipulation */
    seq_t digits[65];        /* 3^40 > 2^64, 65 is safely larger. */
    size_t len = 0;

    while (m != 0) {
        idx_t r = m % 3;      /* remainder 0,1,2 */
        m = m / 3;
        if (r == 0 || r == 1) {
            digits[len++] = (seq_t)r;   /* digit 0 or +1 */
        } else {                       /* r == 2 ⇒ digit -1 with carry 1 */
            digits[len++] = (seq_t)-1;
            m += 1;                    /* carry to next higher digit */
        }
    }

    /* Build the reversed number: sum digits[i] * 3^(len-1-i). */
    seq_t rev = 0;
    seq_t power = pow3(len - 1);   /* 3^(len-1) */
    for (size_t i = 0; i < len; ++i) {
        rev += digits[i] * power;
        power /= 3;                /* move to next lower power of 3 */
    }
    return rev;
}
