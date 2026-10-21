/*
 * A097868  The n-th group (n>=0) of 16 consecutive terms are the entries,
 *          read by rows, of the 4 X 4 matrix A[n]=MA[n-1] (n>=1), where M is
 *          the 4 X 4 matrix [[0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1], [1, 0,
 *          0, 1]] and A[0] is the 4 X 4 matrix [[1, 1, 1, 1], [1, 1, 1, 2],
 *          [1, 1, 2, 3], [1, 2, 3, 4]].
 * https://oeis.org/A097868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the entry (r,c) of the 4×4 matrix A[m] where m = n/16, r
 *            = (n%16)/4, c = n%4. A[0] =
 *            [[1,1,1,1],[1,1,1,2],[1,1,2,3],[1,2,3,4]]. For m≥1: A[m] = M ·
 *            A[m‑1] with M = [[0,1,0,0], [0,0,1,0], [0,0,0,1], [1,0,0,1]].
 *            The matrix is updated row‑wise: new row0 = previous row1, new
 *            row1 = previous ro...
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Number of matrices pre‑computed so far (last index computed). */
    static idx_t last = 0;
    /* Storage for up to 2000 matrices – more than enough for n≤10000. */
    static seq_t A[2000][4][4];

    /* Initialise A[0] once. */
    if (last == 0) {
        A[0][0][0] = 1; A[0][0][1] = 1; A[0][0][2] = 1; A[0][0][3] = 1;
        A[0][1][0] = 1; A[0][1][1] = 1; A[0][1][2] = 1; A[0][1][3] = 2;
        A[0][2][0] = 1; A[0][2][1] = 1; A[0][2][2] = 2; A[0][2][3] = 3;
        A[0][3][0] = 1; A[0][3][1] = 2; A[0][3][2] = 3; A[0][3][3] = 4;
        last = 0; /* A[0] is now ready */
    }

    /* Compute the required matrix if it has not been computed yet. */
    idx_t m = n >> 4;                    /* m = n / 16 */
    if (m > last) {
        for (idx_t k = last + 1; k <= m; ++k) {
            /* new row 0 = previous row 1 */
            A[k][0][0] = A[k-1][1][0];
            A[k][0][1] = A[k-1][1][1];
            A[k][0][2] = A[k-1][1][2];
            A[k][0][3] = A[k-1][1][3];
            /* new row 1 = previous row 2 */
            A[k][1][0] = A[k-1][2][0];
            A[k][1][1] = A[k-1][2][1];
            A[k][1][2] = A[k-1][2][2];
            A[k][1][3] = A[k-1][2][3];
            /* new row 2 = previous row 3 */
            A[k][2][0] = A[k-1][3][0];
            A[k][2][1] = A[k-1][3][1];
            A[k][2][2] = A[k-1][3][2];
            A[k][2][3] = A[k-1][3][3];
            /* new row 3 = previous row 0 + previous row 3 */
            A[k][3][0] = A[k-1][0][0] + A[k-1][3][0];
            A[k][3][1] = A[k-1][0][1] + A[k-1][3][1];
            A[k][3][2] = A[k-1][0][2] + A[k-1][3][2];
            A[k][3][3] = A[k-1][0][3] + A[k-1][3][3];
        }
        last = m;
    }

    /* Pick the entry corresponding to the requested index. */
    idx_t row = (n & 0xF) >> 2; /* (n % 16) / 4 */
    idx_t col = n & 0x3;        /* n % 4 */
    return A[m][row][col];
}
