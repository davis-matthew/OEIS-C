/*
 * A017072  a(n) = (8*n)^8.
 * https://oeis.org/A017072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n)^8
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 31, u128 n <= 8191,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute base = 8*n. */
    seq_t base = (seq_t)8 * (seq_t)n;
    /* Raise base to the 8th power. */
    seq_t result = 1;
    for (int i = 0; i < 8; ++i)
        result *= base;
    return result;
}
