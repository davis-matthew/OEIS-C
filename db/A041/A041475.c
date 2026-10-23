/*
 * A041475  Denominators of continued fraction convergents to sqrt(253).
 * https://oeis.org/A041475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   q(0)=q(1)=1; for n≥2 * q(n)=c[n-1]·q(n-1)+q(n-2), where the
 *            periodic part of the * continued fraction for √253 is * c =
 *            [1,9,1,1,1,2,1,7,4,2,2,2,4,7,1,2,1,1,1,9,1,30] (period 22)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* Periodic continuation coefficients for √253 */
    static const unsigned int period[22] = {
        1, 9, 1, 1, 1, 2, 1, 7, 4, 2, 2, 2, 4, 7, 1, 2, 1, 1, 1, 9, 1, 30
    };
    const unsigned int period_len = 22U;

    /* q(n-2), q(n-1) and q(n) */
    seq_t q_prev2 = (seq_t)1;  /* q(0) */
    seq_t q_prev1 = (seq_t)1;  /* q(1) */
    seq_t q = (seq_t)0;

    for (idx_t k = 2; k <= n; ++k) {
        unsigned int ak = period[(int)((k - 1) % period_len)];
        q = (seq_t)ak * q_prev1 + q_prev2;
        q_prev2 = q_prev1;
        q_prev1 = q;
    }

    return q_prev1;
}
