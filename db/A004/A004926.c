/*
 * A004926  a(n) = floor(n*phi^11), where phi is the golden ratio, A001622.
 * https://oeis.org/A004926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*phi^11) = floor((199*n + sqrt(39605*n^2))/2),
 *            since phi^11 = (199 + 89*sqrt(5))/2
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 1318, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t isqrt_seq(seq_t N, seq_t guess) {
    /* Newton iteration from an upper bound guess >= sqrt(N) */
    seq_t x = guess;
    if (x == 0) return 0;
    for (;;) {
        seq_t y = (x + N / x) / 2;
        if (y >= x) return x;
        x = y;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t m = (seq_t)n;
    seq_t N = (seq_t)39605 * m * m;
    seq_t r = isqrt_seq(N, (seq_t)200 * m);
    return ((seq_t)199 * m + r) / 2;
}
