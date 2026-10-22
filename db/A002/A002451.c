/*
 * A002451  Expansion of 1/((1-x)*(1-4*x)*(1-9*x)).
 * https://oeis.org/A002451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) + c(n), c(n) = 4*c(n-1) + 1, a(0) = 1, c(0) = 1
 *            (c(n) = (4^(n+1)-1)/3, the expansion of 1/((1-x)*(1-4*x)))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t c = (seq_t)1;
    seq_t s = (seq_t)1;
    idx_t i;
    for (i = 1; i <= n; i++) {
        c = (seq_t)4 * c + (seq_t)1;
        s = (seq_t)9 * s + c;
    }
    return s;
}
