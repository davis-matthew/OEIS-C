/*
 * A159083  Products of 7 consecutive integers.
 * https://oeis.org/A159083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)*(n-3)*(n-4)*(n-5)*(n-6)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 26, u64 n <= 568,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 7) return (seq_t)0;
    seq_t result = (seq_t)n;
    for (idx_t i = 1; i < 7; ++i) {
        result *= (seq_t)(n - i);
    }
    return result;
}
