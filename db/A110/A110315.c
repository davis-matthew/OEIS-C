/*
 * A110315  Diagonal sums of the Fibonacci related number triangle A110314.
 * https://oeis.org/A110315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (6-n^2)(-1)^n/8-(n^2+4n-2)/8
 * Range:     i8 n <= 7, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;                  /* convert index to seq_t */
    seq_t n2 = ns * ns;
    seq_t sign = ((n & 1) ? (seq_t)-1 : (seq_t)1);
    seq_t term1 = ((seq_t)6 - n2) * sign;
    seq_t term2 = n2 + (seq_t)4 * ns - (seq_t)2;
    seq_t num   = term1 - term2;
    return num / (seq_t)8;
}
