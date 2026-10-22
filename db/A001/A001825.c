/*
 * A001825  Central factorial numbers: 2nd subdiagonal of A008956.
 * https://oeis.org/A001825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (12*n^2 + 12*n + 11)*a(n-1) - (4*n^2 + 3)*(12*n^2 +
 *            1)*a(n-2) + (2*n - 1)^6*a(n-3), a(0) = 1, with a(-1) = a(-2) = 0
 *            (giving a(1) = 35, a(2) = 1974)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t am2 = 0, am1 = 0, a0 = 1;
    idx_t i;
    for (i = 1; i <= n; i++) {
        seq_t k = (seq_t)i;
        seq_t k2 = k * k;
        seq_t c1 = 12 * k2 + 12 * k + 11;
        seq_t c2 = (4 * k2 + 3) * (12 * k2 + 1);
        seq_t x = 2 * k - 1;
        seq_t x2 = x * x;
        seq_t c3 = x2 * x2 * x2;
        seq_t r = c1 * a0 + c3 * am2;
        r -= c2 * am1;
        am2 = am1;
        am1 = a0;
        a0 = r;
    }
    return a0;
}
