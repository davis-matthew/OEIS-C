/*
 * A118709  a(n) = determinant of n X n circulant matrix whose first row is
 *          the first n cube numbers 0, 1, ..., (n-1)^3.
 * https://oeis.org/A118709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-1)^(n-1)*(n-1)^2*n^(n-2)*(n^(2n) - (n^2-3n-3 +
 *            sqrt(15n^2-18n-9)i)^n - (n^2-3n-3 - sqrt(15n^2-18n-9)i)^n +
 *            (n^2-3n+3)^n)/24
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 4, i64 n <= 7, i128 n <= 11,
 *            big any n
 * Helpers:   big_pow, compute_S_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/compute_S_n.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    seq_t n_val = (seq_t)n;
    /* Compute n^n once to reuse */
    seq_t n_pow_n = big_pow(n_val, (seq_t)n);

    /* n^(2n) = (n^n)^2 */
    seq_t pow_n_2n = n_pow_n * n_pow_n;

    /* n^(n-2) = n^n / n^2 */
    seq_t pow_nm2 = n_pow_n / ((seq_t)n * (seq_t)n);

    /* Compute (n^2 - 3n + 3)^n */
    seq_t base_term = (seq_t)n * (seq_t)n - 3 * (seq_t)n + 3;
    seq_t pow_term = big_pow(base_term, (seq_t)n);

    /* Compute S_n = b^n + c^n */
    seq_t S_n = compute_S_n(n);

    /* Numerator inside the parentheses */
    seq_t num = pow_n_2n - S_n + pow_term;

    /* (n-1)^2 * n^(n-2) * num */
    seq_t n_minus1 = n_val - 1;
    seq_t n_minus1_sq = n_minus1 * n_minus1;
    seq_t prod = pow_nm2 * n_minus1_sq * num;

    /* Apply the sign (-1)^(n-1) */
    if ((n % 2) == 0)
        prod = -prod;

    /* Divide by 24 */
    seq_t result = prod / 24;

    return result;
}
