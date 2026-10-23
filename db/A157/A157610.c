/*
 * A157610  a(n) = 29282*n^2 - 484*n + 1.
 * https://oeis.org/A157610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 29282*n^2 - 484*n + 1
 * Range:     u8 none, u16 n <= 1, u32 n <= 382, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (seq_t)29282 * nn * nn - (seq_t)484 * nn + (seq_t)1;
}
