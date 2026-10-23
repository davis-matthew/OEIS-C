/*
 * A156845  a(n) = 12167*n - 8579.
 * https://oeis.org/A156845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12167*n - 8579
 * Range:     u8 none, u16 n <= 6, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)n * 12167) - 8579;
}
