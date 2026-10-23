/*
 * A158229  a(n) = 225*n + 1.
 * https://oeis.org/A158229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 225*n + 1
 * Range:     u8 n <= 1, u16 n <= 291, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Ensure the multiplication is performed in seq_t to avoid
       overflow of the intermediate idx_t value. */
    return ((seq_t)n * (seq_t)225) + (seq_t)1;
}
