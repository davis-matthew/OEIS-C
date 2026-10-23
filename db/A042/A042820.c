/*
 * A042820  Numerators of continued fraction convergents to sqrt(941).
 * https://oeis.org/A042820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0) = 30; for n>0, a(n) are the numerators of the convergents
 *            to √941. They are computed by the recurrence p_k = a_k · p_{k-1}
 *            + p_{k-2} with p_{-2}=0, p_{-1}=1, where a_0 = 30 and the
 *            partial quotients a_k repeat with period
 *            1,2,11,1,14,2,2,1,1,2,2,14,1,11,2,1,60 (length 17)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 33, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n).  */
seq_t a(idx_t n) {
    const int a0 = 30;

    /* Period of the continued fraction for sqrt(941), computed once. */
    static int period[64];
    static int period_len = 0;
    if (period_len == 0) {
        const int N = 941;
        int m = 0, d = 1, a = a0, pos = 0;
        while (1) {
            m = d * a - m;
            d = (N - m * m) / d;
            a = (a0 + m) / d;
            period[pos++] = a;
            if (a == 2 * a0) break;
        }
        period_len = pos;
    }

    if (n == 0) return (seq_t)a0;

    seq_t p_minus2 = 0;   /* p_{-2} */
    seq_t p_minus1 = 1;   /* p_{-1} */
    seq_t p = 0;
    int a_k;

    for (idx_t k = 0; k <= n; ++k) {
        if (k == 0) {
            a_k = a0;
        } else {
            int idx = (int)((k - 1) % period_len);
            a_k = period[idx];
        }
        p = (seq_t)a_k * p_minus1 + p_minus2;
        p_minus2 = p_minus1;
        p_minus1 = p;
    }
    return p;
}
