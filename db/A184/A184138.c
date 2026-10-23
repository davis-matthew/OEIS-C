/*
 * A184138  Number of n X 3 binary arrays with rows and columns in
 *          nondecreasing order.
 * https://oeis.org/A184138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5040 + 7860*n + 4494*n² + 2044*n³ + 525*n⁴ + 175*n⁵ +
 *            21*n⁶ + n⁷) / 5040
 * Range:     u8 none, u16 n <= 1, u32 n <= 20, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined; return 0 as a guard. */
    if (n == 0) return 0;

    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;           /* n² */
    seq_t n3 = n2 * N;          /* n³ */
    seq_t n4 = n3 * N;          /* n⁴ */
    seq_t n5 = n4 * N;          /* n⁵ */
    seq_t n6 = n5 * N;          /* n⁶ */
    seq_t n7 = n6 * N;          /* n⁷ */

    seq_t num = (seq_t)5040
            + (seq_t)7860 * N
            + (seq_t)4494 * n2
            + (seq_t)2044 * n3
            + (seq_t)525  * n4
            + (seq_t)175  * n5
            + (seq_t)21   * n6
            + n7;

    return num / (seq_t)5040;
}
