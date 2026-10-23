/*
 * A015129  Triangle of (Gaussian) q-binomial coefficients for q = -13.
 * https://oeis.org/A015129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 15, i32 n <= 17, i64 n <= 47, i128 n <= 81,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Implements sequence A015129: the flattening of the triangle of Gaussian q-binomial
 * coefficients with q = -13.  For n ≥ 0 let (r,c) be the 0‑based row and column 
 * such that n = r(r+1)/2 + c, where r is the smallest integer with  
 * r(r+1)/2 > n.  Then
 *     a(n) = T(r,c)  where
 *     T(r,c) = ∏_{i=1..c} ((-13)^(1+r-i)-1)/((-13)^i-1).
 * The product is performed multiplicatively, using big integer arithmetic iff 
 * seq_t is an arbitrary‑precision type.
 */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2  */
    idx_t r = 0;
    idx_t cum = 0;
    while (cum + (r + 1) <= n) {
        cum += r + 1;
        ++r;
    }
    idx_t col = n - cum;                 /* column index within the row */

    /* Handle the trivial case directly */
    if (col == 0)
        return (seq_t)1;

    /* Prepare values for the multiplicative product */
    seq_t pow_nk1 = big_pow((seq_t)-13, (seq_t)r); /* = (-13)^r */
    seq_t pow_k   = (seq_t)1;                     /* = (-13)^0 initially */
    seq_t T       = (seq_t)1;

    /* Iterate over k = 1 .. col */
    for (idx_t k = 1; k <= col; ++k) {
        pow_k   *= (seq_t)-13;      /* pow_k = (-13)^k   */
        seq_t numerator   = pow_nk1 - (seq_t)1;
        seq_t denominator = pow_k - (seq_t)1;
        T = (T * numerator) / denominator;  /* exact division */
        pow_nk1 /= (seq_t)-13;            /* prepare for next k */
    }
    return T;
}
