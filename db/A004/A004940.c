/*
 * A004940  a(n) = round(n*phi^5), where phi is the golden ratio, A001622.
 * https://oeis.org/A004940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n*phi^5) = 11*n + floor((m+1)/2), where phi^5 =
 *            (11+5*sqrt(5))/2 and m = floor(n*(5*sqrt(5)-11)), i.e. the
 *            largest m >= 0 with m*(m+22*n) <= 4*n^2
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t four_n2 = 4 * N * N;
    seq_t t = 22 * N;
    /* initial estimate of n*(5*sqrt(5)-11) = n*0.1803398874989... */
    seq_t m = N * 180339887 / 1000000000;
    while ((m + 1) * (m + 1 + t) <= four_n2)
        m++;
    while (m > 0 && m * (m + t) > four_n2)
        m--;
    return 11 * N + (m + 1) / 2;
}
