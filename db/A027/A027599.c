/*
 * A027599  a(n) = 3*n^2 - 7*n + 6.
 * https://oeis.org/A027599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^2 - 7*n + 6
 * Range:     u8 n <= 10, u16 n <= 148, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 910 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;
    return 3 * nn * nn - 7 * nn + 6;
}
