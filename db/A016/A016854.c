/*
 * A016854  a(n) = (5*n)^6.
 * https://oeis.org/A016854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n)^6
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 8, u64 n <= 325, u128 n <= 9999,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 5*n in seq_t to avoid overflow on very large seq_t types. */
    seq_t m = (seq_t)5 * (seq_t)n;
    /* Multiply six times to get (5*n)^6. */
    return m * m * m * m * m * m;
}
