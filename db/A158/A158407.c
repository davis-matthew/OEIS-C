/*
 * A158407  a(n) = 900*n + 1.
 * https://oeis.org/A158407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 900*n + 1
 * Range:     u8 none, u16 n <= 72, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the generic sequence type before arithmetic */
    return ((seq_t)n * 900) + 1;
}
