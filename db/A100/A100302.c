/*
 * A100302  Expansion of (1 - x - 6*x^2)/((1 - x)*(1 - x - 8*x^2)).
 * https://oeis.org/A100302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1)+7*a(n-2)-8*a(n-3), a(0)=1, a(1)=1, a(2)=3
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    seq_t a0 = (seq_t)1; /* a(n-3) */
    seq_t a1 = (seq_t)1; /* a(n-2) */
    seq_t a2 = (seq_t)3; /* a(n-1) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t ai = 2 * a2 + 7 * a1 - 8 * a0;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
