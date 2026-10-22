/*
 * A005912  Truncated cube numbers.
 * https://oeis.org/A005912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (3*n+1)^3 - 8*n*(n+1)*(n+2)/6
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 550, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_val = (seq_t)n;
    seq_t three_n_plus_1 = 3 * n_val + 1;
    seq_t cube = three_n_plus_1 * three_n_plus_1 * three_n_plus_1;
    seq_t product_div6 = n_val * (n_val + 1) * (n_val + 2) / 6;
    seq_t term = 8 * product_div6;
    return cube - term;
}
