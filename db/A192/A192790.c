/*
 * A192790  Molecular topological index of the Andrásfai graphs.
 * https://oeis.org/A192790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*(3*n-2)*(3*n-1)
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* behaviour below the offset is irrelevant */
    seq_t tn   = (seq_t)n;                  /* cast index to seq_t first */
    seq_t t1   = 3 * tn - 2;                /* 3*n-2 */
    seq_t t2   = 3 * tn - 1;                /* 3*n-1 */
    return 2 * tn * t1 * t2;                /* compute product in seq_t */
}
