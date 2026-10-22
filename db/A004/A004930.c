/*
 * A004930  a(n) = floor(n*phi^15), where phi is the golden ratio, A001622.
 * https://oeis.org/A004930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*phi^15) = 1364*n + floor(n/phi^15), since phi^15
 *            = 682 + 305*sqrt(5) = 1364 + phi^(-15); m = floor(n/phi^15) is
 *            the largest m with m*(1364*n + m) <= n^2
 * Range:     u8 n <= 0, u16 n <= 48, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t nn = N * N;
    seq_t m = N / 1365;
    while ((m + 1) * (N * 1364 + m + 1) <= nn)
        m++;
    return N * 1364 + m;
}
