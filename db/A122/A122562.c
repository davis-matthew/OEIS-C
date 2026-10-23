/*
 * A122562  a(n) = n^3 + 114 * n.
 * https://oeis.org/A122562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 + 114*n
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 1625, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type so arithmetic stays in seq_t */
    seq_t ns = (seq_t)n;
    /* Compute n^3 + 114*n using only seq_t operations */
    return ns * ns * ns + (seq_t)114 * ns;
}
