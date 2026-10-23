/*
 * A168416  a(n) = 1 + 9*floor(n/2).
 * https://oeis.org/A168416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 9*floor(n/2)
 * Range:     u8 n <= 57, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For unsigned integer division, n/2 is already floor(n/2). */
    return (seq_t)1 + (seq_t)9 * ((seq_t)n / (seq_t)2);
}
