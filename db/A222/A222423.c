/*
 * A222423  Sum of (n AND k) for k = 0, 1, 2, ..., n, where AND is the bitwise
 *          AND operator.
 * https://oeis.org/A222423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{k=0..n} (n & k)
 * Range:     u8 n <= 25, u16 n <= 411, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        sum += (seq_t)(n & k);
    }
    return sum;
}
