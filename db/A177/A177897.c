/*
 * A177897  Triangle of octanomial coefficients read by rows: n-th row is
 *          obtained by expanding ((1+x)*(1+x^2)*(1+x^4))^n mod 2 and
 *          converting to decimal.
 * https://oeis.org/A177897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A001317(7*n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* row^ stores the binary coefficient vector of ((1+x)(1+x^2)(1+x^4))^row
       as an integer whose k‑th bit is the parity of the coefficient of x^k.   */
    seq_t row = 1;           /* a(0) */
    if (n == 0) return row;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t next  = 0;
        seq_t shift = row;
        for (int t = 0; t < 8; ++t) {   /* convolve with (1+x+...+x^7) over GF(2) */
            next ^= shift;           /* add shifted row (mod 2) */
            shift <<= 1;             /* shift left by one position */
        }
        row = next;
    }
    return row;
}
