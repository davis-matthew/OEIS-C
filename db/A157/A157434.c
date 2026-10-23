/*
 * A157434  a(n) = 4*n^2 + 79*n + 390.
 * https://oeis.org/A157434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^2 + 79*n + 390 (offset 1)
 * Range:     u8 none, u16 n <= 118, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 4*nn*nn + 79*nn + 390;
}
