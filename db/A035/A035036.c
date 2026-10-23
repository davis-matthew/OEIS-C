/*
 * A035036  Fourier coefficients of E_{gamma,2}*E_{0,4}.
 * https://oeis.org/A035036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -8 * Σ_{d|n} d^5 * (-1)^d
 * Range:     i8 n <= 1, i16 n <= 5, i32 n <= 48, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1; for n ≥ 1 the sum runs over all positive divisors d of n. */

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;
    seq_t res = (seq_t)0;
    idx_t d;
    for (d = 1; d <= n / d; ++d) {           /* iterate all divisor pairs (d, e) */
        if (n % d == 0) {
            idx_t e = n / d;
            /* contribution of divisor d */
            seq_t kd = (seq_t)d;
            seq_t d5 = kd * kd * kd * kd * kd;  /* d^5 */
            seq_t term_d = ((d & 1) ? (seq_t)8 : -(seq_t)8) * d5;
            res += term_d;
            if (e != d) {
                seq_t ke = (seq_t)e;
                seq_t e5 = ke * ke * ke * ke * ke;  /* e^5 */
                seq_t term_e = ((e & 1) ? (seq_t)8 : -(seq_t)8) * e5;
                res += term_e;
            }
        }
    }
    return res;
}
