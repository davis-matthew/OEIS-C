/*
 * A158228  a(n) = 225n^2 + 2n.
 * https://oeis.org/A158228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 225*n*n + 2*n
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 4369, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The calculation is performed in seq_t to avoid overflow in
       intermediate steps when the target type is larger than
       64-bit. */
    seq_t N = (seq_t)n;
    return 225 * N * N + 2 * N;
}
