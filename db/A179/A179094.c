/*
 * A179094  Disorder number of the n X n grid graph.
 * https://oeis.org/A179094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 0 for n=1; n^3 - n - 1 for odd n > 1; n^3 - 3 for even n
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)          /* a(1) = 0 */
        return (seq_t)0;

    seq_t m   = (seq_t)n;   /* cast to seq_t for correct arithmetic */
    seq_t cube = m * m * m; /* compute n^3 */

    if ((n & 1) == 0)          /* even n */
        return cube - (seq_t)3;
    else                       /* odd n > 1 */
        return cube - m - (seq_t)1;
}
