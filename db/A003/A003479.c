/*
 * A003479  Expansion of 1/((1-x)*(1-x-2*x^3)).
 * https://oeis.org/A003479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-3) + 1, with a(0) = 1, a(1) = 2, a(2) = 3
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 41, u64 n <= 83, u128 n <= 167,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x0 = 1, x1 = 2, x2 = 3, nx;
    idx_t i;
    if (n == 0) return x0;
    if (n == 1) return x1;
    if (n == 2) return x2;
    for (i = 3; i <= n; i++) {
        nx = x2 + 2 * x0 + 1;
        x0 = x1;
        x1 = x2;
        x2 = nx;
    }
    return x2;
}
