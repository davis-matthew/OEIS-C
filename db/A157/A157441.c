/*
 * A157441  a(n) = 1331*n - 1122.
 * https://oeis.org/A157441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1331*n - 1122
 * Range:     u8 n <= 1, u16 n <= 50, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)1331 * (seq_t)n) - (seq_t)1122;
}
