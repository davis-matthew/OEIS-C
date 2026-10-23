/*
 * A042046  Numerators of continued fraction convergents to sqrt(547).
 * https://oeis.org/A042046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = numerators of continued fraction convergents to sqrt(547)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 14, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const idx_t D = 547;
    static const idx_t a0 = 23;
    static seq_t period[32];
    static int period_len = 0;

    /* Compute the periodic part of the continued fraction once. */
    if (!period_len) {
        idx_t m = 0, d = 1, a = a0;
        while (1) {
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;
            period[period_len++] = (seq_t)a;
            if (a == 2 * a0) break;
        }
    }

    /* Recurrence for convergent numerators: p_{-2}=0, p_{-1}=1 */
    seq_t p_nm2 = 0;
    seq_t p_nm1 = 1;
    seq_t val = 0;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t ai;
        if (i == 0)
            ai = a0;
        else
            ai = period[(i - 1) % period_len];
        val = ai * p_nm1 + p_nm2;
        p_nm2 = p_nm1;
        p_nm1 = val;
    }
    return val;
}
