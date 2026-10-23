/*
 * A025804  Expansion of 1/((1-x^2)*(1-x^4)*(1-x^9)).
 * https://oeis.org/A025804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + n*(15+9*(-1)^n) + 116 + 64*(-1)^n)/144)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use signed 64‑bit arithmetic for safety; n <= 10000 has
       n^2 <= 1e8, well within 63 bits. */
    int64_t nn = (int64_t)n;
    int sign = (n & 1) ? -1 : 1;          /* (-1)^n  */
    int64_t num = nn * nn                     /* n^2 */
               + nn * (15 + 9 * sign)          /* n*(15+9*(-1)^n) */
               + 116 + 64 * sign;             /* 116 + 64*(-1)^n */
    int64_t res = num / 144;                 /* floor division */
    return (seq_t)res;
}
