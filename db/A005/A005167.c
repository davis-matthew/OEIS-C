/*
 * A005167  a(n+1) = (1 + a(0)^4 + ... + a(n)^4 )/(n+1) (not always
 *          integral!).
 * https://oeis.org/A005167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n+1) = (1 + a(0)^4 + ... + a(n)^4)/(n+1), a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t cur = 1;
    seq_t sum = 2; /* 1 + a(0)^4 */
    for (idx_t i = 1; i <= n; i++) {
        cur = sum / (seq_t)i;
        seq_t sq = cur * cur;
        sum += sq * sq;
    }
    return cur;
}
