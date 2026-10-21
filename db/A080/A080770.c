/*
 * A080770  a(n) = floor(e*(n+3)!) - (n+3)*(n+2)*(n+1)*n*floor(e*(n-1)!).
 * https://oeis.org/A080770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 + 7*n^2 + 17*n + 16 (n >= 2), a(1) = 17
 * Range:     u8 n <= 3, u16 n <= 37, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the special first term */
    if (n == 1)
        return (seq_t)17;

    /* Compute using the cubic polynomial for n >= 2 */
    seq_t N = (seq_t)n;
    return N * N * N + 7 * N * N + 17 * N + 16;
}
