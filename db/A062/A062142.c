/*
 * A062142  Fourth (unsigned) column sequence of coefficient triangle A062137
 *          of generalized Laguerre polynomials n!*L(n,3,x).
 * https://oeis.org/A062142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+3)! * binomial(n+6,6) / 3! (OEIS A062142)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* factorial part: (n+3)! */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n + 3; ++i)
        fact *= (seq_t)i;

    /* binomial part: binomial(n+6,6)   use multiply/divide to keep numbers small */
    seq_t binom = 1;
    idx_t m = n + 6;
    for (idx_t k = 1; k <= 6; ++k) {
        binom *= (seq_t)(m - k + 1);
        binom /= (seq_t)k;   /* exact division */
    }

    /* combine and divide by 3! = 6 */
    seq_t result = fact * binom;
    result /= (seq_t)6;
    return result;
}
