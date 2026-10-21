/*
 * A062317  Numbers k such that 5*k-1 is a perfect square.
 * https://oeis.org/A062317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n*(n-1) + 5 - (6*n-3)*(-1)^n) / 8 (valid for n >= 1,
 *            OEIS A062317)
 * Range:     u8 n <= 5, u16 n <= 111, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1, 0 is undefined */
    seq_t nn = (seq_t)n;             /* convert index to seq_t once */

    /* term1 = 10 * n * (n-1) */
    seq_t term1 = (seq_t)10 * nn * (nn - 1);

    /* term2 = 5 */
    seq_t term2 = (seq_t)5;

    /* term3 = (6*n - 3) * (-1)^n
       Compute (6*n-3) using seq_t arithmetic to avoid overflow */
    seq_t six_n_minus_three = (seq_t)6 * nn - (seq_t)3;
    seq_t sign = (n % 2 == 0) ? (seq_t)1 : (seq_t)-1;  /* (-1)^n */
    seq_t term3 = six_n_minus_three * sign;

    seq_t numerator = term1 + term2 - term3;

    return numerator / (seq_t)8;
}
