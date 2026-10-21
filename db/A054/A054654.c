/*
 * A054654  Triangle of Stirling numbers of 1st kind, S(n, n-k), n >= 0, 0 <=
 *          k <= n.
 * https://oeis.org/A054654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign, easy, nice
 * Formula:   a(n) = S(r,r-k) where r = floor((sqrt(8n+1)-1)/2), k = n -
 *            r(r+1)/2. S are signed Stirling numbers of the first kind
 * Range:     i8 n <= 23, i16 n <= 49, i32 n <= 112, i64 n <= 247,
 *            i128 n <= 656, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* determine the row r of the flattened triangle */
    idx_t r = 0;
    while (1) {
        __int128 t = (__int128)(r + 1) * (r + 2) / 2;
        if (t > n) break;
        ++r;
    }

    /* column index within that row   */
    idx_t col   = n - r * (r + 1) / 2;
    /* position in the Stirling row   */
    idx_t target = r - col;

    /* allocate the row array (zero‑initialised) */
    seq_t *A = (seq_t *)calloc((size_t)(r + 1), sizeof(seq_t));
    if (!A) return (seq_t)0;                 /* allocation failure */

    A[0] = (seq_t)1;                         /* S(0,0)=1 */

    /* build rows 1 … r using backwards in‑place update */
    for (idx_t row = 1; row <= r; ++row) {
        for (idx_t k = row; k >= 1; --k) {
            A[k] = A[k - 1] - (seq_t)(row - 1) * A[k];
        }
        A[0] = (seq_t)0;                     /* S(row,0)=0 for row>0 */
    }

    seq_t result = A[target];
    free(A);
    return result;
}
