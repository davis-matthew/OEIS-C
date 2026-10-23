/*
 * A163660  Number of reduced words of length n in Coxeter group on 38
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 36*a(n-1)+36*a(n-2)+36*a(n-3)+36*a(n-4)-666*a(n-5) for n
 *            >= 6, with initial values: a(0)=1, a(1)=38, a(2)=1406,
 *            a(3)=52022, a(4)=1924814, a(5)=71217415
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial terms up to a(5) */
    static const seq_t init[6] = {
        (seq_t)1,
        (seq_t)38,
        (seq_t)1406,
        (seq_t)52022,
        (seq_t)1924814,
        (seq_t)71217415
    };

    if (n < 6) return init[n];

    /* sliding window of the last 5 terms: a(n-5) ... a(n-1) */
    seq_t last[5];
    last[0] = init[1];
    last[1] = init[2];
    last[2] = init[3];
    last[3] = init[4];
    last[4] = init[5];

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next =
            36 * last[4] + 36 * last[3] + 36 * last[2] +
            36 * last[1] - 666 * last[0];

        /* shift window */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = last[4];
        last[4] = next;
    }

    return last[4];
}
