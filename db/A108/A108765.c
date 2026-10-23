/*
 * A108765  Expansion of g.f. (1 - x + x^2)/((1-3*x)*(x-1)^2).
 * https://oeis.org/A108765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) + n, a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t res = 1;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        res = res * 3 + (seq_t)i;   /* a(i) = 3*a(i-1) + i */
    }
    return res;
}
