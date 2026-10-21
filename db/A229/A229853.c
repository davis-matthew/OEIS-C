/*
 * A229853  a(n) = 384*n + 1.
 * https://oeis.org/A229853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 384*n + 1
 * Range:     u8 n <= 0, u16 n <= 170, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)384 * (seq_t)n) + (seq_t)1;
}
