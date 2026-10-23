/*
 * A174305  Values of the family of polynomials y = x^n + 1 at point x =
 *          discriminant of y.
 * https://oeis.org/A174305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ( (n mod 4) == 3 ? -(n^(n^2)) : n^(n^2) ) + 1
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 3, i64 n <= 5, i128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here n^(n^2) is computed with big_pow.                                             */
#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute n^n^2 using big_pow: base = n, exponent = n*n */
    seq_t base = (seq_t)n;
    idx_t exp_idx = n * n;                     /* n^2 fits in idx_t for all n < 2^32 */
    seq_t exp   = (seq_t)exp_idx;              /* exponent as seq_t */

    seq_t val = big_pow(base, exp);            /* val = n^(n^2) */

    /* Apply the sign for n ≡ 3 (mod 4) */
    if ((n % 4) == 3)
        val = -val;                            /* negate */

    /* finally add 1 */
    return val + (seq_t)1;
}
