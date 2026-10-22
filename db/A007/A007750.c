/*
 * A007750  Nonnegative integers n such that n^2*(n+1)*(2*n+1)^2*(7*n+1)/36 is
 *          a square.
 * https://oeis.org/A007750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-2) - a(n-4) + 8, a(0) = 0, a(1) = 1, a(2) = 7,
 *            a(3) = 24
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t w0 = 0, w1 = 1, w2 = 7, w3 = 24;
    idx_t i;
    if (n == 0) return w0;
    if (n == 1) return w1;
    if (n == 2) return w2;
    if (n == 3) return w3;
    for (i = 4; i <= n; i++) {
        seq_t next = w2 * 16 - w0 + 8;
        w0 = w1;
        w1 = w2;
        w2 = w3;
        w3 = next;
    }
    return w3;
}
