/*
 * A041352  Numerators of continued fraction convergents to sqrt(190).
 * https://oeis.org/A041352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0)=13; for n>0, a(n) = a(n-1)*q_n + a(n-2) where q_0=13 and
 *            the subsequent q_n are the partial quotients of sqrt(190):
 *            1,3,1,1,1,2,2,2,1,1,1,3,1,26 repeating
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 26, u64 n <= 53, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Partial quotients of the continued fraction for sqrt(190) */
    static const seq_t q[15] = {
        (seq_t)13,
        (seq_t)1, (seq_t)3, (seq_t)1, (seq_t)1, (seq_t)1,
        (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)1, (seq_t)1,
        (seq_t)1, (seq_t)3, (seq_t)1, (seq_t)26
    };
    static const idx_t period = 14;   /* number of repeating partial quotients after the first term */

    seq_t p_im2 = 0;   /* p(-2) */
    seq_t p_im1 = 1;   /* p(-1) */
    seq_t p = 0;       /* p(k) */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t qk;
        if (k == 0) {
            qk = q[0];
        } else {
            qk = q[1 + ( (k - 1) % period )];
        }
        p = qk * p_im1 + p_im2;
        p_im2 = p_im1;
        p_im1 = p;
    }
    return p;
}
