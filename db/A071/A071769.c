/*
 * A071769  Determinant of the n X n matrix whose element (i,j) equals |i-j|
 *          (mod 4).
 * https://oeis.org/A071769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(4k+1) = -2^(4k+2) * k * (3k-2) a(4k+2) = -2^(4k) *
 *            (12k+1) a(4k+3) = 2^(4k+2) * (12k^2+4k+1) a(4k+4) = -2^(4k+2) *
 *            (12k+3) (k ≥ 0) a(0) = 1
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 24, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow2_exp(idx_t e)
{
    /* Compute 2^e by exponentiation by squaring.  Works for all
       seq_t types that support multiplication. */
    seq_t result = 1;
    seq_t base   = 2;
    while (e > 0) {
        if (e & 1) result = result * base;
        base = base * base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    idx_t q = n / 4;
    switch (n % 4) {
    case 1: { // a(4k+1)
        idx_t k = q;
        seq_t pow = pow2_exp(4*k + 2);
        seq_t term = pow * (seq_t)k * (seq_t)(3*k - 2);
        return -term;
    }
    case 2: { // a(4k+2)
        idx_t k = q;
        seq_t pow = pow2_exp(4*k);
        seq_t term = pow * (seq_t)(12*k + 1);
        return -term;
    }
    case 3: { // a(4k+3)
        idx_t k = q;
        seq_t pow = pow2_exp(4*k + 2);
        seq_t term = pow * (seq_t)(12*k*k + 4*k + 1);
        return term;
    }
    case 0: { // a(4k+4)  (n > 0)
        idx_t k = q - 1;          /* since n = 4*(k+1) */
        seq_t pow = pow2_exp(4*k + 2);
        seq_t term = pow * (seq_t)(12*k + 3);
        return -term;
    }
    }
    /* unreachable */
    return (seq_t)0;
}
