/*
 * A194431  a(n) = 8*n^2 - 6*n - 1.
 * https://oeis.org/A194431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n^2 - 6*n - 1
 * Range:     u8 n <= 6, u16 n <= 90, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    return 8 * tn * tn - 6 * tn - 1;
}
