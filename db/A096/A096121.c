/*
 * A096121  Number of full spectrum rook's walks on a (2 X n) board.
 * https://oeis.org/A096121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n+1) = n*(n+1)*(a(n) + a(n-1)), for n > 1, with initial values
 *            a(1) = 2, a(2) = 8
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)8;

    seq_t a_prev2 = (seq_t)2;  /* a(1) */
    seq_t a_prev1 = (seq_t)8;  /* a(2) */

    for (idx_t k = 3; k <= n; ++k)
    {
        seq_t sum = a_prev1 + a_prev2;          /* a(k-1) + a(k-2) */
        sum = sum * (seq_t)(k - 1);             /* multiply by n = k-1 */
        sum = sum * (seq_t)k;                   /* multiply by n+1 = k    */
        a_prev2 = a_prev1;                      /* shift for next iteration */
        a_prev1 = sum;
    }

    return a_prev1;  /* a(n) */
}
