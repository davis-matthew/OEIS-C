/*
 * A166189  Number of 3 X 3 X 3 triangular nonnegative integer arrays with all
 *          sums of an element and its neighbors <= n.
 * https://oeis.org/A166189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/1920)*(4*n^6 + 72*n^5 + 530*n^4 + 2040*n^3 + 4296*n^2
 *            * + 4608*n + 1905 + 15*(-1)^n)
 * Range:     u8 none, u16 n <= 2, u32 n <= 29, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;           /* promote to seq_t early */
    seq_t n2 = N * N;             /* n^2 */
    seq_t n3 = n2 * N;            /* n^3 */
    seq_t n4 = n3 * N;            /* n^4 */
    seq_t n5 = n4 * N;            /* n^5 */
    seq_t n6 = n5 * N;            /* n^6 */

    /* polynomial part */
    seq_t term = 4 * n6
               + 72 * n5
               + 530 * n4
               + 2040 * n3
               + 4296 * n2
               + 4608 * N
               + 1905;

    /* add 15*(-1)^n */
    if ((n & 1) == 0)
        term += 15;     /* n even -> (-1)^n = 1 */
    else
        term -= 15;     /* n odd  -> (-1)^n = -1 */

    /* exact division by 1920 */
    return term / 1920;
}
