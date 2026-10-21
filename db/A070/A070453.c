/*
 * A070453  a(n) = n^2 mod 31.
 * https://oeis.org/A070453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod 31
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 31 first to keep intermediate values small. */
    idx_t r = n % 31;
    seq_t sq = (seq_t)r * (seq_t)r;
    return sq % 31;
}
