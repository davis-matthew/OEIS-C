/*
 * A060422  Number of acute triangles made from vertices of a regular n-gon.
 * https://oeis.org/A060422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)*(n-1)/24 if n is odd, a(n) = n*(n-2)*(n-4)/24 if
 *            n is even
 * Range:     u8 n <= 18, u16 n <= 116, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    if (n & 1) {
        /* n odd: N*(N+1)*(N-1)/24 */
        return N * (N + 1) * (N - 1) / 24;
    } else {
        /* n even: N*(N-2)*(N-4)/24 */
        return N * (N - 2) * (N - 4) / 24;
    }
}
