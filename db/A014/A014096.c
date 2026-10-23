/*
 * A014096  Inverse of 87th cyclotomic polynomial.
 * https://oeis.org/A014096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in (1 - x^3 - x^29 + x^32) / ((1 -
 *            x)(1 - x^87))
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* base counts for the factor (1 - x)^{-1} */
    idx_t q0    = n / 87;               /* floor(n/87)   */
    seq_t cnt0  = (seq_t)q0 + 1;        /* always present */

    /* counts for the other factors
       (1 - x^3)    → subtract
       (1 - x^29)   → subtract
       (1 - x^32)   → add
    */
    seq_t cnt32 = 0, cnt3 = 0, cnt29 = 0;

    if (n >= 32) {
        cnt32 = (seq_t)((n - 32) / 87) + 1;
    }
    if (n >= 3) {
        cnt3 = (seq_t)((n - 3) / 87) + 1;
    }
    if (n >= 29) {
        cnt29 = (seq_t)((n - 29) / 87) + 1;
    }

    return cnt0 + cnt32 - cnt3 - cnt29;
}
