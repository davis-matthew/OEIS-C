/*
 * A182866  Number of edges in the n^2 X n^2 Sudoku graph.
 * https://oeis.org/A182866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^4*(-1 - 2*n + 3*n^2))/2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned __int128 sn = (unsigned __int128)n;
    unsigned __int128 sn2 = sn * sn;                  /* n^2  */
    unsigned __int128 sn4 = sn2 * sn2;                /* n^4  */
    unsigned __int128 t   = (unsigned __int128)3 * sn2 /* 3n^2 */
                          - (unsigned __int128)2 * sn /* -2n  */
                          - (unsigned __int128)1; /* -1   */
    unsigned __int128 prod = sn4 * t;                 /* n^4 * t */
    prod /= 2;                                       /* divide by 2 */

    return (seq_t)prod;
}
