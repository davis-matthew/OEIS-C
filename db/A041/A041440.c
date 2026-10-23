/*
 * A041440  Numerators of continued fraction convergents to sqrt(236).
 * https://oeis.org/A041440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent of sqrt(236)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define PERIOD_LEN 12

seq_t a(idx_t n)
{
    /* Periodic continued fraction terms after the initial term a0 = 15. */
    static const seq_t period[PERIOD_LEN] = {
        (seq_t)2, (seq_t)1, (seq_t)3, (seq_t)5,
        (seq_t)1, (seq_t)6, (seq_t)1, (seq_t)5,
        (seq_t)3, (seq_t)1, (seq_t)2, (seq_t)30
    };

    seq_t p_im2 = 0;   /* p_{-2} */
    seq_t p_im1 = 1;   /* p_{-1} */
    seq_t p_i   = 0;   /* current numerator */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t a_i;
        if (i == 0) {
            a_i = (seq_t)15;              /* a0 */
        } else {
            int pos = (int)((i - 1) % PERIOD_LEN);
            a_i = period[pos];
        }
        p_i = a_i * p_im1 + p_im2;
        p_im2 = p_im1;
        p_im1 = p_i;
    }
    return p_i;
}
