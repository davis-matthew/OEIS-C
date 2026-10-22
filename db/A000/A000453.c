/*
 * A000453  Stirling numbers of the second kind, S(n,4).
 * https://oeis.org/A000453
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = S(n,4) via the triangle recurrence S(m,k) = k*S(m-1,k) +
 *            S(m-1,k-1), with S(1,1) = 1 and S(1,k) = 0 for k > 1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t s1 = 1, s2 = 0, s3 = 0, s4 = 0;
    idx_t m;
    for (m = 2; m <= n; m++) {
        s4 = (seq_t)4 * s4 + s3;
        s3 = (seq_t)3 * s3 + s2;
        s2 = (seq_t)2 * s2 + s1;
    }
    return s4;
}
