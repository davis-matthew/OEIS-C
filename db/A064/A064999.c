/*
 * A064999  Partial sums of sequence (essentially A002378): 1, 2, 6, 12, 20,
 *          30, 42, 56, 72, 90, ...
 * https://oeis.org/A064999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^3 + 3*n^2 + 2*n + 3)/3
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t num = ns * ns * ns
                + (seq_t)3 * ns * ns
                + (seq_t)2 * ns
                + (seq_t)3;
    return num / (seq_t)3;
}
