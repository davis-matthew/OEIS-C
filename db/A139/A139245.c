/*
 * A139245  a(n) = 20*n - 16.
 * https://oeis.org/A139245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = 20*n - 16
 * Range:     u8 n <= 13, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)n * (seq_t)20) - (seq_t)16;
}
