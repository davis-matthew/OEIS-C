/*
 * A063289  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          16 ).
 * https://oeis.org/A063289
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = floor((9*n-22)/2) for n >= 3, a(2) = -1
 * Range:     i8 n <= 30, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 2) return (seq_t)-1;
    if (n < 2)  return (seq_t)0;   /* undefined below offset */

    return ((seq_t)9 * (seq_t)n - (seq_t)22) / (seq_t)2;
}
