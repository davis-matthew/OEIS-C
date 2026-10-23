/*
 * A157949  a(n) = 128*n - 1.
 * https://oeis.org/A157949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 128*n - 1
 * Range:     u8 n <= 2, u16 n <= 512, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Ensure the multiplication is performed in seq_t to avoid
       intermediate overflow on unsigned types. */
    return ((seq_t)n * (seq_t)128u) - (seq_t)1u;
}
