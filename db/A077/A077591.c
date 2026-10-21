/*
 * A077591  Maximum number of regions into which the plane can be divided
 *          using n (concave) quadrilaterals.
 * https://oeis.org/A077591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n^2 - 8*n + 2 for n > 0; a(0) = 1
 * Range:     u8 n <= 6, u16 n <= 91, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t ns = (seq_t)n;
    return (seq_t)8 * ns * ns - (seq_t)8 * ns + (seq_t)2;
}
