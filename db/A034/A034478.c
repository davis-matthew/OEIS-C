/*
 * A034478  a(n) = (5^n + 1)/2.
 * https://oeis.org/A034478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5^n + 1)/2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 5^n iteratively. */
    seq_t pow5 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow5 *= 5;
    }
    /* Return (5^n + 1)/2. */
    return (pow5 + 1) / 2;
}
