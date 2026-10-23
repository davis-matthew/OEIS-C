/*
 * A184637  a(n) = floor(1/{(n^4+3*n)^(1/4)}), where {}=fractional part.
 * https://oeis.org/A184637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + floor(n^2/3) for n>2; a(1)=2; a(2)=6
 * Range:     u8 n <= 13, u16 n <= 221, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;

    seq_t nn = n;                /* convert index to seq_t first */
    seq_t sq = nn * nn;          /* n^2 */
    return sq + (sq / 3);        /* n^2 + floor(n^2/3) */
}
