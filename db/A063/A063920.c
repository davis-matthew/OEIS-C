/*
 * A063920  Numbers k such that k = 2*phi(k) + phi(phi(k)).
 * https://oeis.org/A063920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12-2*(-1)^n) * 2^floor(n/2)
 * Range:     u8 n <= 9, u16 n <= 25, u32 n <= 57, u64 n <= 121, u128 n <= 249,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* coefficient: 10 for even n, 14 for odd n */
    seq_t coeff = (n & 1) ? (seq_t)14 : (seq_t)10;

    /* exponent: floor(n/2) */
    idx_t exp = n >> 1;

    /* compute 2^exp by repeated left shift */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < exp; ++i)
        pow2 <<= 1;

    return coeff * pow2;
}
