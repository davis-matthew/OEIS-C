/*
 * A229789  Triangle T(n,k): the coefficient [x^(n-k)] of the polynomial
 *          2^n*n!*L(n,3/2,x), where L is the generalized Laguerre Polynomial
 *          in the Abramowitz-Stegun normalization.
 * https://oeis.org/A229789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = coefficient [x^(n-k)] of 2^n*n!*L(n,3/2,x) read row‑wise
 * Range:     i8 n <= 7, i16 n <= 17, i32 n <= 49, i64 n <= 130, i128 n <= 372,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Let r be the largest integer with r(r+1)/2 ≤ n and c = n - r(r+1)/2.  */
/*   Then a(n) = (-1)^(r-c) * 2^(r-c) * C(r,c) * Π_{i=0}^{c-1} (2r+3-2i).  */
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Find the row r and the index c within that row.          */
    idx_t     r = 0, t = 0;
    while (t + r + 1 <= n) {
        r++;
        t += r;          /* t = r(r+1)/2 after the loop.                */
    }
    idx_t c = n - t;     /* 0 ≤ c ≤ r.                                   */

    /* Sign factor: (-1)^(r-c).                                */
    seq_t sign = ((r - c) & 1) ? (seq_t)-1 : (seq_t)1;

    /* Power of two: 2^(r-c).                                   */
    seq_t pow2 = pow_seq((seq_t)2, (int)(r - c));

    /* Binomial coefficient C(r,c).                              */
    seq_t binom = (seq_t)1;
    for (idx_t i = 1; i <= c; ++i) {
        binom = binom * (seq_t)(r - i + 1);
        binom = binom / (seq_t)i;            /* exact division.               */
    }

    /* Product of odd numbers: Π_{i=0}^{c-1} (2r+3-2i).           */
    seq_t prod = (seq_t)1;
    for (idx_t i = 0; i < c; ++i) {
        prod = prod * (seq_t)(2 * r + 3 - 2 * i);
    }

    return sign * pow2 * binom * prod;
}
