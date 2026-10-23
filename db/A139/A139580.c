/*
 * A139580  a(n) = n*(2*n + 17).
 * https://oeis.org/A139580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(2*n+17)
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to the appropriate seq_t type before any arithmetic */
    seq_t ns = (seq_t)n;
    /* Compute 2*ns + 17 in seq_t */
    seq_t t = (seq_t)(ns * (seq_t)2 + (seq_t)17);
    /* Return the product ns * t */
    return ns * t;
}
