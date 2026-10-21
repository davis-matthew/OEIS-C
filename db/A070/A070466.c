/*
 * A070466  a(n) = n^2 mod 44.
 * https://oeis.org/A070466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod 44
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Work modulo 44 first to avoid overflow */
    idx_t r = n % 44ULL;          /* r < 44, fits in even 8 bits */
    seq_t sq = (seq_t)r * (seq_t)r;
    return sq % 44ULL;
}
