/*
 * A189836  a(n) = n^2 + 11.
 * https://oeis.org/A189836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + 11
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n*n + 11 using seq_t for all arithmetic */
    return (seq_t)n * (seq_t)n + (seq_t)11;
}
