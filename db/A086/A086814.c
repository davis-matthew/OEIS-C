/*
 * A086814  a(n) = ceiling( (1 + n + 2*n^2 + 4*n^3)/(1 + 2*n + n^2) ).
 * https://oeis.org/A086814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil( (1 + n + 2*n^2 + 4*n^3) / (1 + 2*n + n^2) )
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type */
    seq_t sn = n;
    /* Pre‑compute powers of n */
    seq_t n2 = sn * sn;     /* n^2 */
    seq_t n3 = n2 * sn;     /* n^3 */

    /* Numerator: 1 + n + 2*n^2 + 4*n^3 */
    seq_t num = 1 + sn + n2 * 2 + n3 * 4;

    /* Denominator: 1 + 2*n + n^2 */
    seq_t den = 1 + sn * 2 + n2;

    /* Ceil division: (num + den - 1) / den */
    return (num + den - 1) / den;
}
