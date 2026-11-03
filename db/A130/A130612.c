/*
 * A130612  Sum of the first 10^n squares.
 * https://oeis.org/A130612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*10^(3*n)+3*10^(2*n)+10^n)/6
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 10 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Compute 10^n, 10^(2n), and 10^(3n) */
    seq_t t   = pow10(n);
    seq_t t2  = pow10(2*n);
    seq_t t3  = pow10(3*n);

    /* numerator: 2*10^(3n) + 3*10^(2n) + 10^n */
    seq_t num = 2*t3 + 3*t2 + t;

    /* exact division by 6 gives the sum of squares */
    return num / 6;
}
