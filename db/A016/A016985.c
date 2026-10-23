/*
 * A016985  a(n) = (7n)^5.
 * https://oeis.org/A016985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7n)^5
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 12, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n * 7;
    seq_t res = 1;
    for (int i = 0; i < 5; ++i) {
        res *= m;
    }
    return res;
}
