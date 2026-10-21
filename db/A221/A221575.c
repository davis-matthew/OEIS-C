/*
 * A221575  Number of 0..n arrays of length 5 with each element differing from
 *          at least one neighbor by something other than 1.
 * https://oeis.org/A221575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^5 + n^4 - 2*n^3 + 12*n^2 - 15*n + 9 (for n > 2)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 84, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 6;
    if (n == 2) return 57;
    seq_t N = n;
    seq_t res = N * N * N * N * N;   /* n^5 */
    res += N * N * N * N;            /* + n^4 */
    res -= 2 * N * N * N;            /* - 2 n^3 */
    res += 12 * N * N;               /* + 12 n^2 */
    res -= 15 * N;                   /* - 15 n */
    res += 9;                        /* + 9 */
    return res;
}
