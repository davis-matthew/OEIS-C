/*
 * A166797  Number of nX2 1..4 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          nondecreasing order.
 * https://oeis.org/A166797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^6 + 18*n^5 + 115*n^4 + 90*n^3 - 746*n^2 + 702*n - 180)
 *            / 180
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 37, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;       /* n^2 */
    seq_t N3 = N2 * N;      /* n^3 */
    seq_t N4 = N3 * N;      /* n^4 */
    seq_t N5 = N4 * N;      /* n^5 */
    seq_t N6 = N5 * N;      /* n^6 */

    seq_t ans = N6;
    ans += (seq_t)18 * N5;
    ans += (seq_t)115 * N4;
    ans += (seq_t)90  * N3;
    ans -= (seq_t)746 * N2;
    ans += (seq_t)702 * N;
    ans -= (seq_t)180;
    ans /= (seq_t)180;

    return ans;
}
