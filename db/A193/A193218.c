/*
 * A193218  Number of vertices in truncated tetrahedron with faces that are
 *          centered polygons.
 * https://oeis.org/A193218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n^3 - 9*n^2 + 5*n - 1
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 895, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn  = (seq_t)n;         /* n as seq_t */
    seq_t n2  = tn * tn;          /* n^2 */
    seq_t n3  = n2 * tn;          /* n^3 */
    seq_t res = 6 * n3 - 9 * n2 + 5 * tn - 1; /* 6*n^3 - 9*n^2 + 5*n - 1 */
    return res;
}
