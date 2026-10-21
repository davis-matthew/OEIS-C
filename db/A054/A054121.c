/*
 * A054121  T(n,n-3), array T as in A054120.
 * https://oeis.org/A054121
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3/2 * (3*n^3 - 30*n^2 + 107*n - 134) (n > 3) a(3) = 1
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 3)
        return (seq_t)1;

    seq_t N = (seq_t)n;

    /* compute the polynomial 3*n^3 - 30*n^2 + 107*n - 134 */
    seq_t poly = N * N * N * (seq_t)3;         /* 3*n^3 */
    poly -= N * N * (seq_t)30;                /* -30*n^2 */
    poly += N * (seq_t)107;                   /* +107*n */
    poly -= (seq_t)134;                       /* -134 */

    poly = poly * (seq_t)3;                   /* multiply by 3 */
    return poly / (seq_t)2;                   /* divide by 2 */
}
