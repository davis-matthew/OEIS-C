/*
 * A228405  Pellian Array, A(n, k) with numbers m such that 2*m^2 +- 2^k is a
 *          square, and their corresponding square roots, read downward by
 *          diagonals.
 * https://oeis.org/A228405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 118, u32 n <= 403, u64 n <= 1429, u128 none,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: A(0,k) = (k odd ? 2^((k-1)/2) : 0),
   A(1,0) = 1,  A(1,k) = A(1,k-1)+A(0,k-1)  (k≥1),
   A(n,k) = 2*A(n-1,k)+A(n-2,k)              (n≥2).
   The sequence a(n) is A(n',k') read by the successive anti‑diagonals
   of the array (n',k').  Initial values: A(0,0)=0, A(0,1)=1, A(1,0)=1.   */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow2.h"

/* Static cache: one big square matrix for the first (curD+1) rows/cols  */
static seq_t *matrix = NULL;  /* row‑major storage: index = row*(curD+1)+col   */
static idx_t curD   = 0;      /* maximum diagonal that has been built          */

/* Allocate and fill the array up to diagonal newD (inclusive).        */
/* Only elements needed for the first newD+1 rows and columns are set. */
static void build_matrix(idx_t newD)
{
    /* Allocate new storage  */
    size_t size = (size_t)(newD + 1) * (newD + 1);
    seq_t *newM = (seq_t *)malloc(size * sizeof(seq_t));
    if (!newM) return;    /* out of memory – caller must handle it */

    /* --- First row A(0,k)  ------------------------------------------- */
    for (idx_t k = 0; k <= newD; ++k)
        newM[0 * (newD + 1) + k] = (k & 1ULL) ? pow2((k - 1) / 2) : (seq_t)0;

    /* --- Second row A(1,k)  ------------------------------------------ */
    newM[1 * (newD + 1) + 0] = 1;
    for (idx_t k = 1; k <= newD; ++k)
        newM[1 * (newD + 1) + k] =
            newM[1 * (newD + 1) + k - 1] + newM[0 * (newD + 1) + k - 1];

    /* --- Remaining rows n ≥ 2  (Pell recurrence per column) ---------- */
    for (idx_t k = 0; k <= newD; ++k) {
        for (idx_t n = 2; n <= newD; ++n) {
            newM[n * (newD + 1) + k] =
                2 * newM[(n - 1) * (newD + 1) + k] + newM[(n - 2) * (newD + 1) + k];
        }
    }

    /* Replace old matrix, free old memory.                              */
    free(matrix);
    matrix = newM;
    curD   = newD;
}

seq_t a(idx_t n)
{
    /* Determine which anti‑diagonal contains the index n.             */
    idx_t d = 0;
    /* d satisfies d*(d+1)/2 ≤ n < (d+1)*(d+2)/2                           */
    while ((d + 1) * (d + 2) / 2 <= n) ++d;

    /* Ensure the matrix is large enough.                               */
    if (!matrix || d > curD) build_matrix(d);

    /* Position of the entry inside the diagonal.                       */
    idx_t start = d * (d + 1) / 2;
    idx_t pos   = n - start;                /* 0 … d  */
    /* Entry is at (row = pos, column = d-pos).                         */
    return matrix[pos * (curD + 1) + (d - pos)];
}
