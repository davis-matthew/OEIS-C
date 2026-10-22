/*
 * A007845  Least positive integer k for which 5^n divides k!.
 * https://oeis.org/A007845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(0) = 1; a(n) = least multiple k of 5 with sum_{i>=1}
 *            floor(k/5^i) >= n, found by stepping k in multiples of 5 and
 *            adding the 5-adic valuation of each
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t target = (seq_t)n;
    seq_t count = 0;
    seq_t k = 0;
    while (count < target) {
        k += 5;
        seq_t m = k;
        while (m % 5 == 0) {
            count += 1;
            m /= 5;
        }
    }
    return k;
}
