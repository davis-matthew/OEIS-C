/*
 * A042144  Numerators of continued fraction convergents to sqrt(597).
 * https://oeis.org/A042144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = numerator of the n-th convergent to sqrt(597) using
 *            recurrence p_{-2}=0, p_{-1}=1 and p_k = a_k * p_{k-1} + p_{k-2},
 *            where a_0 = 24 and the continued fraction of sqrt(597) repeats
 *            with period [2,3,3,1,3,1,2,11,1,6,16,6,1,11,2,1,3,1,3,3,2,48]
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const unsigned int cf[22] = {
        2,3,3,1,3,1,2,11,1,6,
        16,6,1,11,2,1,3,1,3,3,
        2,48
    };
    const unsigned int period = 22;

    if (n == 0) return (seq_t)24;

    seq_t p_minus = 1;
    seq_t p = 24;

    for (idx_t i = 1; i <= n; ++i) {
        unsigned int ai = cf[(i - 1) % period];
        seq_t ai_seq = (seq_t)ai;
        seq_t p_new = ai_seq * p + p_minus;
        p_minus = p;
        p = p_new;
    }
    return p;
}
