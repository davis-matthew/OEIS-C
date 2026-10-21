/*
 * A064096  Fifth diagonal of triangle A064094.
 * https://oeis.org/A064096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 3*n + 5*n^2 + 5*n^3
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 950, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    /* Compute the polynomial using seq_t arithmetic to avoid overflow */
    return (seq_t)1 + (seq_t)3 * sn
           + (seq_t)5 * sn * sn
           + (seq_t)5 * sn * sn * sn;
}
