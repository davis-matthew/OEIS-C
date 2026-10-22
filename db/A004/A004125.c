/*
 * A004125  Sum of remainders of n mod k, for k = 1, 2, 3, ..., n.
 * https://oeis.org/A004125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{i=1..n} (n mod i)
 * Range:     u8 n <= 38, u16 n <= 608, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; k++) {
        sum += (seq_t)n % (seq_t)k;
    }
    return sum;
}
