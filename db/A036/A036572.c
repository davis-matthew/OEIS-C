/*
 * A036572  Number of tetrahedra in largest triangulation of polygonal prism
 *          with regular polygonal base.
 * https://oeis.org/A036572
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n*n + 6*n - 13)/4 (valid for n >= 3)
 * Range:     u8 n <= 13, u16 n <= 451, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For any n < 3 the sequence is undefined; return 0. */
    if (n < 3) return (seq_t)0;

    seq_t nn = (seq_t)n;                    /* cast index to seq_t */
    seq_t numerator = nn * nn              /* n^2   */
                    + (seq_t)6 * nn         /* 6n    */
                    - (seq_t)13;            /* -13   */

    return numerator / (seq_t)4;           /* integer division */
}
