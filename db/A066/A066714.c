/*
 * A066714  Coordination sequence for ReO_3 net with respect to Re atom.
 * https://oeis.org/A066714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1, a(n) = n^2+2 if n is even, a(n)=3n^2+3 if n is
 *            odd
 * Range:     u8 n <= 10, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if ((n & 1) == 0)          /* even */
        return (seq_t)n * (seq_t)n + (seq_t)2;
    else                       /* odd */
        return (seq_t)3 * (seq_t)n * (seq_t)n + (seq_t)3;
}
