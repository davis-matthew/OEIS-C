/*
 * A064586  Order of twisted group 2F4(2^(2*n + 1)).
 * https://oeis.org/A064586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(2*n+1))^12 * ((2^(2*n+1))-1) * ((2^(2*n+1))^3+1) *
 *            ((2^(2*n+1))^4-1) * ((2^(2*n+1))^6+1)
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 0, u128 n <= 1,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* exponent e = 2*n+1 for q = 2^e */
    idx_t e = 2 * n + 1;

    /* compute q = 2^e and q12 = 2^(12*e) using shifts */
    seq_t one = (seq_t)1;
    seq_t q   = one << e;
    seq_t q12 = one << (e * 12);

    /* auxiliary powers of q without external helpers */
    seq_t q3 = q * q * q;   /* q^3 */
    seq_t q4 = q3 * q;      /* q^4 */
    seq_t q6 = q3 * q3;     /* q^6 */

    /* assemble the product */
    seq_t res = q12;
    res *= (q - 1);      /* (q - 1)   */
    res *= (q3 + 1);     /* (q^3 + 1) */
    res *= (q4 - 1);     /* (q^4 - 1) */
    res *= (q6 + 1);     /* (q^6 + 1) */

    return res;
}
