/*
 * A004963  a(n) = ceiling(n*phi^8), where phi is the golden ratio, A001622.
 * https://oeis.org/A004963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(n*phi^8), with phi^8 = (47 + 21*sqrt(5))/2, so
 *            a(0) = 0 and, for n > 0, a(n) = floor((47*n +
 *            floor(sqrt(2205*n^2)))/2) + 1
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 2155, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t isqrt_seq(seq_t N, seq_t start) {
    /* Newton's method from an upper bound start >= sqrt(N) */
    seq_t x = start;
    if (x == 0) return 0;
    seq_t y = (x + N / x) / 2;
    while (y < x) {
        x = y;
        y = (x + N / x) / 2;
    }
    return x;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t m = (seq_t)n;
    seq_t N = m * m * 2205;          /* (21*n)^2 * 5 */
    seq_t s = isqrt_seq(N, m * 47);  /* 47*n > sqrt(2205)*n */
    return (m * 47 + s) / 2 + 1;
}
