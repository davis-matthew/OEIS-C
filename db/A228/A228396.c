/*
 * A228396  The number of permutations of length n sortable by 2 reversals.
 * https://oeis.org/A228396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 + n*(n^3 - 2*n^2 + 2*n - 19)/6
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    seq_t N = (seq_t)n;

    seq_t n2 = N * N;          /* n^2 */
    seq_t n3 = n2 * N;         /* n^3 */
    seq_t n4 = n3 * N;         /* n^4 */

    /* numerator = n^4 - 2*n^3 + 2*n^2 - 19*n + 48 */
    seq_t num = n4;
    num -= (seq_t)2 * n3;
    num += (seq_t)2 * n2;
    num -= (seq_t)19 * N;
    num += (seq_t)48;

    seq_t result = num / (seq_t)6;
    return result;
}
