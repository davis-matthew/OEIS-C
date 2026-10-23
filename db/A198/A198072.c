/*
 * A198072  Round((n+1/n)^7).
 * https://oeis.org/A198072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round((n + 1/n)^7)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 21, u64 n <= 565,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t sn = (seq_t)n;          /* n as seq_t */

    /* powers of n */
    seq_t n2 = sn * sn;           /* n^2 */
    seq_t n3 = n2 * sn;           /* n^3 */
    seq_t n4 = n2 * n2;           /* n^4 */
    seq_t n5 = n4 * sn;           /* n^5 */
    seq_t n6 = n3 * n3;           /* n^6 */
    seq_t n7 = n6 * sn;           /* n^7 */

    /* integer part: n^7 + 7*n^5 + 21*n^3 + 35*n */
    seq_t I = n7;
    I += (seq_t)7 * n5;
    I += (seq_t)21 * n3;
    I += (seq_t)35 * sn;

    /* fractional part: (35/n + 21/n^3 + 7/n^5 + 1/n^7) */
    /* common numerator and denominator */
    seq_t num_F   = (seq_t)35 * n6;      /* 35*n^6 */
    num_F += (seq_t)21 * n4;             /* +21*n^4 */
    num_F += (seq_t)7  * n2;             /* +7*n^2 */
    num_F += (seq_t)1;                  /* +1 */

    seq_t denom_F = n7;                  /* n^7 */

    /* round the fractional part: floor((num_F + denom_F/2)/denom_F) */
    seq_t R = (num_F + (denom_F >> 1)) / denom_F;

    return I + R;
}
