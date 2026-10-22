/*
 * A006101  Gaussian binomial coefficient [ n,3 ] for q=3.
 * https://oeis.org/A006101
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [n,3]_3 via q-Pascal recurrence [m,k] = [m-1,k-1] +
 *            3^k*[m-1,k], [m,0] = 1, [0,k] = 0 for k > 0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t b1 = 0, b2 = 0, b3 = 0;
    idx_t m;
    for (m = 1; m <= n; m++) {
        b3 = b2 + (seq_t)27 * b3;
        b2 = b1 + (seq_t)9 * b2;
        b1 = (seq_t)1 + (seq_t)3 * b1;
    }
    return b3;
}
