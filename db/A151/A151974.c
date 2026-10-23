/*
 * A151974  a(n) = n*(n+1)*(n+2)*(n+3)*(n+4)/8.
 * https://oeis.org/A151974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(n+4)/8
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t prod = nn * ((nn + 1) * ((nn + 2) * ((nn + 3) * (nn + 4))));
    return prod / (seq_t)8;
}
