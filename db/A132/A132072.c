/*
 * A132072  Row sums of triangle A132071.
 * https://oeis.org/A132072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 2^n + n^2 + 2*n - 1 for n > 0
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Compute 2^n in seq_t */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow2 *= (seq_t)2;

    seq_t nn = (seq_t)n;
    return pow2 + nn * nn + (seq_t)2 * nn - (seq_t)1;
}
