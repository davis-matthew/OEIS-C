/*
 * A107731  Row 7 of the array in A107735.
 * https://oeis.org/A107731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + (n^2+n)*(n^2+n+1)*10/3
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * ((seq_t)n + 1);    /* m = n*(n+1) */
    seq_t t = m * (m + 1);                  /* t = m*(m+1) */
    return (t / 3) * 10 + 1;
}
