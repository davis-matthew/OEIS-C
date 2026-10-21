/*
 * A057587  Nonnegative numbers of form n*(n^2+-1)/2.
 * https://oeis.org/A057587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + 6n^2 + 8n)/16 for n even (n^3 + 3n^2 + 7n + 5)/16
 *            for n odd
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    if ((n & 1) == 0) { /* even n */
        seq_t t1 = N * N * N;       /* n^3 */
        seq_t t2 = 6 * N * N;       /* 6n^2 */
        seq_t t3 = 8 * N;           /* 8n */
        return (t1 + t2 + t3) / 16;
    } else { /* odd n */
        seq_t t1 = N * N * N;       /* n^3 */
        seq_t t2 = 3 * N * N;       /* 3n^2 */
        seq_t t3 = 7 * N;           /* 7n */
        return (t1 + t2 + t3 + 5) / 16;
    }
}
