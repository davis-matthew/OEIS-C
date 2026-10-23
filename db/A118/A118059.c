/*
 * A118059  a(n) = 288*n^2 - 168*n - 119.
 * https://oeis.org/A118059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 288*n^2 - 168*n - 119
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A118059 with offset 1.
   a(1) = 1, a(2) = 697, a(3) = 1969, etc. */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 288 * nn * nn - 168 * nn - 119;
}
