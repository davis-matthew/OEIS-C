/*
 * A157803  a(n) = 8984250*n - 8464830.
 * https://oeis.org/A157803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8984250*n - 8464830
 * Range:     u8 none, u16 none, u32 n <= 478, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * (seq_t)8984250 - (seq_t)8464830;
}
