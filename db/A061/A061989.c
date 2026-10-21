/*
 * A061989  Number of ways to place 3 nonattacking queens on a 3 X n board.
 * https://oeis.org/A061989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-3)*(n^2-6*n+12), a(0)=a(1)=a(2)=0
 * Range:     u8 n <= 9, u16 n <= 43, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 3 the value is explicitly 0 */
    if (n < 3)
        return 0;

    /* Promote index to seq_t for arithmetic */
    seq_t nn = (seq_t)n;

    /* Compute the expression (nn - 3) * (nn*nn - 6*nn + 12) */
    seq_t part = nn * nn - 6 * nn + 12;
    return (nn - 3) * part;
}
