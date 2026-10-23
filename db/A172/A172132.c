/*
 * A172132  Number of ways to place 2 nonattacking knights on an n X n board.
 * https://oeis.org/A172132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n - 1)*(n + 4)*(n^2 - 3*n + 4)/2
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 293, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t n1   = nn - 1;           /* n - 1 */
    seq_t n4   = nn + 4;           /* n + 4 */
    seq_t term = nn * nn;          /* n^2 */
    term += 4;                     /* n^2 + 4 */
    term -= 3 * nn;                /* n^2 - 3*n + 4 */
    seq_t prod = n1 * n4;          /* (n-1)*(n+4) */
    prod *= term;                  /* (n-1)*(n+4)*(n^2 - 3*n + 4) */
    prod /= 2;                     /* divide by 2 */
    return prod;
}
