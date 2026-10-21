/*
 * A064587  a(n) = n^6*(n^4 + n^2 + 1)*(n^3 - 1)*(n - 1).
 * https://oeis.org/A064587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6*(n^4 + n^2 + 1)*(n^3 - 1)*(n - 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 23, u128 n <= 400,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;          /* n^2 */
    seq_t n3 = n2 * N;         /* n^3 */
    seq_t n4 = n2 * n2;        /* n^4 */
    seq_t n6 = n3 * n3;        /* n^6 */

    seq_t res = n6 * (n4 + n2 + 1); /* n^6 * (n^4 + n^2 + 1) */
    res *= (n3 - 1);          /* multiply by (n^3 - 1) */
    res *= (N - 1);           /* multiply by (n - 1) */
    return res;
}
