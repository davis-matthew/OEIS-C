/*
 * A064046  Number of length 6 walks on an n-dimensional hypercubic lattice
 *          starting and finishing at the origin and staying in the
 *          nonnegative part.
 * https://oeis.org/A064046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n*(3*n^2 - 3*n + 1)
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 659, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;
    seq_t part = (seq_t)3 * nn * nn - (seq_t)3 * nn + (seq_t)1;
    return (seq_t)5 * nn * part;
}
