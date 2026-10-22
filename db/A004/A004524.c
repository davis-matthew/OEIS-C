/*
 * A004524  Three even followed by one odd.
 * https://oeis.org/A004524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - 1 - cos(Pi*(n-2)/2))/2
 * Range:     u8 n <= 368, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    static const int subtract[] = {0, 1, 2, 1};
    return (n_seq - (seq_t)subtract[n % 4]) / 2;
}
