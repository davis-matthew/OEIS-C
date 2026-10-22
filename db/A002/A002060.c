/*
 * A002060  Number of partitions of an n-gon into (n-5) parts.
 * https://oeis.org/A002060
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2n-7, n-7) * (n-3)*(n-4) / 3
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 18, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 7) return 0;
    idx_t m = 2 * n - 7;
    idx_t k = n - 7;
    seq_t b = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        b = b * (seq_t)(m - k + i) / (seq_t)i;
    }
    idx_t x = (n - 3) * (n - 4);
    if (x % 3 == 0) {
        return b * (seq_t)(x / 3);
    }
    return (b / 3) * (seq_t)x;
}
