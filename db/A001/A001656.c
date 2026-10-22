/*
 * A001656  Fibonomial coefficients.
 * https://oeis.org/A001656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 15*a(n-2) - 15*a(n-3) - 5*a(n-4) + a(n-5),
 *            a(0) = 1, a(1) = 5, a(2) = 40, a(3) = 260, a(4) = 1820 (from
 *            G.f. 1/(1-5*x-15*x^2+15*x^3+5*x^4-x^5))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t v[5];
    v[0] = 1; v[1] = 5; v[2] = 40; v[3] = 260; v[4] = 1820;
    if (n < 5) return v[n];
    for (idx_t i = 5; i <= n; i++) {
        seq_t next = (seq_t)5 * v[4] + (seq_t)15 * v[3] + v[0];
        next -= (seq_t)15 * v[2];
        next -= (seq_t)5 * v[1];
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = next;
    }
    return v[4];
}
