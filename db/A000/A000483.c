/*
 * A000483  Associated Stirling numbers: second-order reciprocal Stirling
 *          numbers (Fekete) a(n) = [[n, 3]]. The number of 3-orbit
 *          permutations of an n-set with at least 2 elements in each orbit.
 * https://oeis.org/A000483
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(n,3), where d(n,k) = (n-1)*(d(n-1,k) + d(n-2,k-1)),
 *            d(0,0) = 1, d(1,k) = 0, d(n,0) = 0 for n >= 1 (associated
 *            Stirling numbers of the first kind)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* p* = row m-2, c* = row m-1 */
    seq_t p0 = (seq_t)1, p1 = (seq_t)0, p2 = (seq_t)0;
    seq_t c0 = (seq_t)0, c1 = (seq_t)0, c2 = (seq_t)0, c3 = (seq_t)0;
    idx_t m;
    if (n < 2) return (seq_t)0;
    for (m = 2; m <= n; m++) {
        seq_t f = (seq_t)(m - 1);
        seq_t n1 = f * (c1 + p0);
        seq_t n2 = f * (c2 + p1);
        seq_t n3 = f * (c3 + p2);
        p0 = c0; p1 = c1; p2 = c2;
        c0 = (seq_t)0; c1 = n1; c2 = n2; c3 = n3;
    }
    return c3;
}
