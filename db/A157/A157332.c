/*
 * A157332  Denominators of Egyptian fraction for Pi/16 based on Machin's
 *          formula.
 * https://oeis.org/A157332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, sign, easy
 * Formula:   a(2n) = (2*n+1) * 5^(2*n+1) * (-1)^n a(2n+1) = -4 * (2*n+1) *
 *            239^(2*n+1) * (-1)^n where n is a non‑negative integer
 *            (triangular indexing)
 * Range:     i8 n <= 0, i16 n <= 2, i32 n <= 4, i64 n <= 8, i128 n <= 16,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* separate even and odd indices */
    if ((n & 1ULL) == 0) {                 /* n = 2*m  */
        idx_t m = n >> 1;                 /* m = n/2  */
        seq_t sign = ((m & 1ULL) == 0) ? (seq_t)1 : (seq_t)-1;  /* (-1)^m */

        seq_t power = big_pow((seq_t)5, (seq_t)(2*m + 1));
        seq_t term  = (seq_t)(2*m + 1) * power * sign;          /* result */
        return term;
    }
    else {                                 /* n = 2*m + 1 */
        idx_t m = n >> 1;                 /* floor(n/2) */
        seq_t sign = ((m & 1ULL) == 0) ? (seq_t)1 : (seq_t)-1;  /* (-1)^m */

        seq_t coeff = (seq_t)-4 * sign;    /* -4 * (-1)^m */
        seq_t power = big_pow((seq_t)239, (seq_t)(2*m + 1));
        seq_t term  = coeff * (seq_t)(2*m + 1) * power;          /* result */
        return term;
    }
}
