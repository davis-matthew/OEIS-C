/*
 * A227121  Number of n X 2 0,1 arrays indicating 2 X 2 subblocks of some
 *          larger (n+1) X 3 binary array having a sum of zero, with rows and
 *          columns of the latter in lexicographically nondecreasing order.
 * https://oeis.org/A227121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 - 2*n^3 + 11*n^2 + 62*n)/24
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t n4 = n3 * N;
    seq_t num = n4 - (seq_t)2 * n3 + (seq_t)11 * n2 + (seq_t)62 * N;
    seq_t res = num / (seq_t)24;
    return res;
}
