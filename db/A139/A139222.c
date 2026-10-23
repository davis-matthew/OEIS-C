/*
 * A139222  a(n) = 30*n - 27.
 * https://oeis.org/A139222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30*n - 27
 * Range:     u8 n <= 9, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute using the formula; n is unsigned, seq_t is unsigned or big int */
    return (seq_t)30 * (seq_t)n - (seq_t)27;
}
