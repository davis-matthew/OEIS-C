/*
 * A002454  Central factorial numbers: a(n) = 4^n * (n!)^2.
 * https://oeis.org/A002454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n * (n!)^2 = Product_{k=1..n} (2*k)^2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t r = 1;
    idx_t k;
    for (k = 1; k <= n; k++) {
        seq_t t = (seq_t)k * 2;
        r *= t;
        r *= t;
    }
    return r;
}
