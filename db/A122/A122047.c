/*
 * A122047  Degree of the polynomial P(n,x), defined by a Somos-6 type
 *          sequence: P(n,x)=(x^(n-1)*P(n-1,x)*P(n-5,x) +
 *          P(n-2,x)*P(n-4,x))/P(n-6,x), initialized with P(n,x)=1 at n<0.
 * https://oeis.org/A122047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n^3 + 6*n^2 + 5*n)/30)
 * Range:     u8 n <= 4, u16 n <= 38, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t t = N * N * N + (seq_t)6 * N * N + (seq_t)5 * N;
    return t / (seq_t)30;
}
