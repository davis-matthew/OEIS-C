/*
 * A153808  8 times octagonal numbers: 8*n*(3*n-2).
 * https://oeis.org/A153808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*n*(3*n-2) = 24*n^2 - 16*n a(0) = 0, a(1) = 8, etc
 * Range:     u8 n <= 3, u16 n <= 52, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 24 * nn * nn - 16 * nn;
}
