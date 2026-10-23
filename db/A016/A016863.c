/*
 * A016863  a(n) = (5*n + 1)^3.
 * https://oeis.org/A016863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n + 1)^3
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 324, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t, then compute (5*n + 1) */
    seq_t base = (seq_t)n * 5 + 1;
    /* Compute the cube */
    seq_t result = base * base * base;
    return result;
}
