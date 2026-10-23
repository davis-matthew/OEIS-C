/*
 * A135951  Central terms of triangle A135950, the matrix inverse of triangle
 *          A022166.
 * https://oeis.org/A135951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * 2^{n(n-1)/2} * C_2(2n,n) where C_2(2n,n) is the
 *            Gaussian binomial coefficient [2n,n] for q = 2. This is
 *            equivalent to the central entry of the matrix inverse of the
 *            Gaussian binomial triangle (A022166)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 4, i64 n <= 6, i128 n <= 9,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Gaussian binomial coefficient [N,K]_q with q = 2 */
static seq_t gaussian_binom_2(idx_t N, idx_t K)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i) {
        seq_t numer = pow2(N - i + 1) - 1;     /* 2^{N-i+1} – 1 */
        seq_t denom = pow2(i) - 1;             /* 2^{i} – 1   */
        res = (res * numer) / denom;          /* exact division */
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Compute C_2(2n,n) */
    seq_t C = gaussian_binom_2(2 * n, n);

    /* 2^{n(n-1)/2} */
    idx_t exp = n * (n - 1) / 2;
    seq_t pow2_term = pow2(exp);

    /* Result = C * 2^exp with sign (-1)^n */
    seq_t result = C * pow2_term;
    if (n & 1)  /* n odd -> negative sign */
        result = -result;

    return result;
}
