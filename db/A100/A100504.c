/*
 * A100504  a(n) = (4*n^3 + 6*n^2 + 8*n + 6)/3.
 * https://oeis.org/A100504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 + 6*n^2 + 8*n + 6)/3
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t = 4*nn*nn*nn + 6*nn*nn + 8*nn + 6;
    return t / 3;
}
