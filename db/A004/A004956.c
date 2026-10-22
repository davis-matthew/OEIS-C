/*
 * A004956  a(n) = ceiling(n*phi), where phi is the golden ratio, A001622.
 * https://oeis.org/A004956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceiling(n*phi), where phi = (1+sqrt(5))/2
 * Range:     u8 n <= 7, u16 n <= 558, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t isqrt(seq_t x) {
    if (x == 0) return 0;
    seq_t y = x;
    seq_t z = (y + 1) / 2;
    while (z < y) {
        y = z;
        z = (y + x / y) / 2;
    }
    return y;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t n_seq = (seq_t)n;
    seq_t sqrt5_n2 = isqrt(5 * n_seq * n_seq);
    return (n_seq + sqrt5_n2) / 2 + 1;
}
