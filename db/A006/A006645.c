/*
 * A006645  Self-convolution of Pell numbers (A000129).
 * https://oeis.org/A006645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 4*a(n-3) - a(n-4), a(0) = 0, a(1) =
 *            0, a(2) = 1, a(3) = 4 (from G.f. (x/(1 - 2*x - x^2))^2,
 *            denominator 1 - 4x + 2x^2 + 4x^3 + x^4)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 48, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t a0 = 0, a1 = 0, a2 = 1, a3 = 4; /* a(i), a(i+1), a(i+2), a(i+3) */
    idx_t i;
    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;
    for (i = 4; i <= n; i++) {
        seq_t pos = 4 * a3;
        seq_t neg = 2 * a2 + 4 * a1 + a0;
        seq_t next = pos - neg;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
