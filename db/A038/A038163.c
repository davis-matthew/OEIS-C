/*
 * A038163  G.f.: 1/((1-x)*(1-x^2))^3.
 * https://oeis.org/A038163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/3840)*(4*n^5 + 90*n^4 + 760*n^3 + 2970*n^2 + 5266*n +
 *            3285 + (-1)^n*(30*n^2 + 270*n + 555)) Equivalent to: For even n
 *            = 2*k: a(n) = (4*k+5)*C(k+4,4)/5 For odd n = 2*k+1: a(n) =
 *            (15+4*k)*C(k+4,4)/5 C(m,4) = m*(m-1)*(m-2)*(m-3)/24
 * Range:     u8 n <= 4, u16 n <= 21, u32 n <= 251, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to seq_t first */
    seq_t n_seq = (seq_t)n;          /* ensure we use seq_t arithmetic */

    /* compute k = n/2 (integer division) */
    seq_t k = n_seq >> 1;            /* same as n_seq / 2 */

    /* compute C(k+4,4) = (k+1)(k+2)(k+3)(k+4)/24
       do the multiplications/divisions in a sequence that keeps
       intermediate results integral.  */
    seq_t binom = k + 1;                 /* start with (k+1) */
    binom = binom * (k + 2) / 2;          /* multiply by (k+2) and divide by 2 */
    binom = binom * (k + 3) / 3;          /* multiply by (k+3) and divide by 3 */
    binom = binom * (k + 4) / 4;          /* multiply by (k+4) and divide by 4 */

    /* decide parity */
    if ((n_seq & 1) == 0) {
        /* even case: a(2*k) = (4*k+5)*binom / 5 */
        seq_t num = (4 * k + 5) * binom;
        return num / 5;
    } else {
        /* odd case: a(2*k+1) = (15+4*k)*binom / 5 */
        seq_t num = (15 + 4 * k) * binom;
        return num / 5;
    }
}
