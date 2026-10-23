/*
 * A042804  Numerators of continued fraction convergents to sqrt(933).
 * https://oeis.org/A042804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent of the continued *
 *            fraction expansion of sqrt(933). The recurrence uses the *
 *            periodic coefficients of the continued fraction of sqrt(933) *
 *            (period length 8: 1,1,5,20,5,1,1,60). Initial values: * a(0) =
 *            30. For n ≥ 1, * let k = n-1 (zero‑based index in the...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 27, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)30;          // a(0)

    static const unsigned int period[8] = {1, 1, 5, 20, 5, 1, 1, 60};
    seq_t p_prev2 = (seq_t)1;     // p_{-1}
    seq_t p_prev1 = (seq_t)30;    // p_{0}

    for (idx_t i = 1; i <= n; ++i) {
        seq_t coeff = (seq_t)period[(i - 1) % 8];
        seq_t p = coeff * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p;
    }
    return p_prev1;
}
