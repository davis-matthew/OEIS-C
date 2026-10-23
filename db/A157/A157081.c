/*
 * A157081  a(n) = 32805000*n^2 + 16200*n + 1.
 * https://oeis.org/A157081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32805000*n^2 + 16200*n + 1
 * Range:     u8 none, u16 none, u32 n <= 11, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = (seq_t)32805000 * nn * nn + (seq_t)16200 * nn + 1;
    return val;
}
