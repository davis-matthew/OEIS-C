/*
 * A144532  Continued fraction for sqrt(8/9).
 * https://oeis.org/A144532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   A144532 - Continued fraction coefficients for sqrt(8/9). a(0) =
 *            0, a(1) = 1, and for n >= 2 the coefficients repeat 16, 2
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    /* For n >= 2, a(n) = 16 when (n-2) is even, otherwise 2. */
    return (((n - 2) & (idx_t)1) ? (seq_t)2 : (seq_t)16);
}
