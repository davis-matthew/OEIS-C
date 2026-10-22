/*
 * A004953  a(n) = round(n*phi^18), where phi is the golden ratio, A001622.
 * https://oeis.org/A004953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n*phi^18) = 5778*n - round(n/phi^18), since phi^18
 *            = (L(18) + F(18)*sqrt(5))/2 with L(18)=5778, F(18)=2584 and
 *            phi^18 + phi^-18 = 5778. round(n/phi^18) = smallest k >= 0 with
 *            (2k+1)^2 + 4n^2 < 2*5778*n*(2k+1)
 * Range:     u8 n <= 0, u16 n <= 11, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t L = 5778;
    seq_t k;
    if (n == 0) return 0;
    /* n/phi^18 is slightly larger than n/5778; start just below */
    k = N / L;
    if (k > 0) k--;
    for (;;) {
        seq_t m = 2 * k + 1;
        /* n/phi^18 < k + 1/2  <=>  m^2 + 4 n^2 < 2 L n m  (using L^2 - 5 F^2 = 4) */
        if (m * m + 4 * N * N < 2 * L * N * m) break;
        k++;
    }
    return L * N - k;
}
