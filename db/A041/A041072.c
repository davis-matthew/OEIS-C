/*
 * A041072  Numerators of continued fraction convergents to sqrt(43).
 * https://oeis.org/A041072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0)=6, a(1)=7. For n≥2 a(n) = c_n * a(n-1) + a(n-2), where c_n
 *            are the partial quotients of sqrt(43): c_0 = 6, c_1 = 1, c_2 =
 *            1, c_3 = 3, c_4 = 1, c_5 = 5, c_6 = 1, c_7 = 3, c_8 = 1, c_9 =
 *            1, c_10 = 12, and then the sequence repeats periodically with
 *            period 10
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 24, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 6;
    static const idx_t period[10] = {
        1, 1, 3, 1, 5, 1, 3, 1, 1, 12
    };
    seq_t p_minus2 = 0;
    seq_t p_minus1 = 1;
    seq_t p_curr;
    idx_t period_idx = 0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t ak;
        if (k == 0) {
            ak = 6;           /* a_0 */
        } else {
            ak = period[period_idx++];
            if (period_idx == 10) period_idx = 0;   /* repeat */
        }

        p_curr = ak * p_minus1 + p_minus2;
        if (k == n) return p_curr;

        p_minus2 = p_minus1;
        p_minus1 = p_curr;
    }

    return 0;   /* never reached */
}
