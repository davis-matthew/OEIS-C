/*
 * A153151  Rotated binary decrementing: For n<2 a(n) = n, if n=2^k, a(n) =
 *          2*n-1, otherwise a(n) = n-1.
 * https://oeis.org/A153151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=0, a(1)=1; For n ≥ 2, a(n) = 2*n−1 if n is a power of 2,
 *            otherwise a(n) = n−1
 * Range:     u8 n <= 255, u16 n <= 2047, u32 n <= 2047, u64 n <= 2047,
 *            u128 n <= 2047, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)n;

    /* Test if n is a power of two: n & (n-1) == 0 for n > 0. */
    if ((n & (n - 1)) == 0) {
        seq_t nn = (seq_t)n;
        return nn * 2 - 1;
    }

    return (seq_t)n - 1;
}
