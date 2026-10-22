/*
 * A002447  Expansion of 1/(1-2*x^2-3*x^3).
 * https://oeis.org/A002447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) + 3*a(n-3), a(0) = 1, a(1) = 0, a(2) = 2
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 36, u64 n <= 70, u128 n <= 140,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x0 = 1, x1 = 0, x2 = 2;  /* a(i), a(i+1), a(i+2) */
    idx_t i;
    if (n == 0) return x0;
    if (n == 1) return x1;
    if (n == 2) return x2;
    for (i = 3; i <= n; i++) {
        seq_t next = (seq_t)2 * x1 + (seq_t)3 * x0;
        x0 = x1;
        x1 = x2;
        x2 = next;
    }
    return x2;
}
