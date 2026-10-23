/*
 * A042426  Numerators of continued fraction convergents to sqrt(741).
 * https://oeis.org/A042426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0)=27; for n≥1, * a(n) = c_n*a(n‑1) + a(n‑2) where c_n are the
 *            periodic CF * coefficients of √741: [4,1,1,13,18,13,1,1,4,54]
 *            (period 10)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 13, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const int period[10] = {4, 1, 1, 13, 18, 13, 1, 1, 4, 54};
    const int L = 10;

    seq_t p_prev2 = 0;   /* p_{-2} */
    seq_t p_prev1 = 1;   /* p_{-1} */
    seq_t p = 0;         /* current numerator */

    for (idx_t i = 0; i <= n; ++i) {
        int ai;
        if (i == 0) {
            ai = 27;
        } else {
            ai = period[(i - 1) % L];
        }
        p = (seq_t)ai * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p;
    }
    return p;
}
