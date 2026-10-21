/*
 * A080260  a(n)=1+(1/12)(n*(n+1)*(n+3)*(4-n)).
 * https://oeis.org/A080260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 1 + (n*(n+1)*(n+3)*(4-n))/12
 * Range:     i8 n <= 4, i16 n <= 13, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t prod = nn * (nn + 1) * (nn + 3) * ((seq_t)4 - nn);
    return 1 + prod / (seq_t)12;
}
