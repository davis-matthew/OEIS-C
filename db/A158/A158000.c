/*
 * A158000  a(n) = 338*n + 1.
 * https://oeis.org/A158000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 338*n + 1
 * Range:     u8 none, u16 n <= 193, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 339, a(2) = 677, ... */
    /* Computation is O(1) and valid for any n >= 1. */
    seq_t nn = (seq_t)n;
    return nn * (seq_t)338 + 1;
}
