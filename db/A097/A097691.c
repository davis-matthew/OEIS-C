/*
 * A097691  Denominators of the continued fraction n-1/(n-1/...) [n times].
 * https://oeis.org/A097691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) is the denominator of the n‑times continued fraction
 *            n-1/(n-1/…/(n-1/n)), computed by the recurrence p_1 = n, q_1 = 1
 *            for i = 2 … n: p_i = n*p_{i-1} - q_{i-1}, q_i = p_{i-1} a(n) =
 *            q_n. This produces the denominators of A097691
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t p = (seq_t)n;      /* numerator of current convergent */
    seq_t q = 1;             /* denominator of current convergent */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t new_p = (seq_t)n * p - q;
        seq_t new_q = p;
        p = new_p;
        q = new_q;
    }
    return q;                /* a(n) is the final denominator */
}
