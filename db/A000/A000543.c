/*
 * A000543  Number of inequivalent ways to color vertices of a cube using at
 *          most n colors.
 * https://oeis.org/A000543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1*C(n,1) + 21*C(n,2) + 267*C(n,3) + 1718*C(n,4) +
 *            5250*C(n,5) + 7980*C(n,6) + 5880*C(n,7) + 1680*C(n,8)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 23, u64 n <= 380, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const unsigned a000543_coef[9] = {0, 1, 21, 267, 1718, 5250, 7980, 5880, 1680};

seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t c = 1; /* C(n, k) */
    idx_t k;
    for (k = 1; k <= 8 && k <= n; k++) {
        c = c * (seq_t)(n - k + 1) / (seq_t)k;
        sum += c * (seq_t)a000543_coef[k];
    }
    return sum;
}
