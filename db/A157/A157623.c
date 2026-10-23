/*
 * A157623  a(n) = 781250*n^2 - 455000*n + 66249.
 * https://oeis.org/A157623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 781250*n^2 - 455000*n + 66249
 * Range:     u8 none, u16 none, u32 n <= 74, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 781250*nn*nn - 455000*nn + 66249;
}
