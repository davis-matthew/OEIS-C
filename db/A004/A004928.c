/*
 * A004928  a(n) = floor(n*phi^13), where phi is the golden ratio, A001622.
 * https://oeis.org/A004928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*phi^13) = floor((521*n +
 *            floor(sqrt(5*(233*n)^2)))/2), since phi^13 = (521 +
 *            233*sqrt(5))/2
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 502, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t isqrt_seq(seq_t N) {
    seq_t x, y;
    if (N < 2) return N;
    x = N;
    y = x / 2 + 1;
    while (y < x) {
        x = y;
        y = (x + N / x) / 2;
    }
    return x;
}

seq_t a(idx_t n) {
    seq_t m = (seq_t)n * 233;
    seq_t s = isqrt_seq(m * m * 5);
    return ((seq_t)n * 521 + s) / 2;
}
