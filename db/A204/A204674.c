/*
 * A204674  a(n) = 4*n^3 + 5*n^2 + 2*n + 1.
 * https://oeis.org/A204674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^3 + 5*n^2 + 2*n + 1
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = 4*nn*nn*nn + 5*nn*nn + 2*nn + 1;
    return val;
}
