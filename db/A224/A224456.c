/*
 * A224456  The Wiener index of the cyclic phenylene with n hexagons (n>=3).
 * https://oeis.org/A224456
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*n*(n*n + 4*n - 4)
 * Range:     u8 none, u16 n <= 18, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_val = (seq_t)n;
    seq_t n_sq  = n_val * n_val;
    seq_t inner = n_sq + 4 * n_val - 4;
    return 9 * n_val * inner;
}
