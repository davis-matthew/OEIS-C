/*
 * A093654  Lower triangular matrix, read by rows, defined as the convergent
 *          of the concatenation of matrices using the iteration: M(n+1) =
 *          [[M(n),0*M(n)],[M(n)^2,M(n)^2]], with M(0) = [1].
 * https://oeis.org/A093654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A093654 –  Lower triangular matrix we built recursively.
 * The matrix M(k) (size 2^k) is defined by:
 *   M(0) = [1]
 *   M(k+1) = [[M(k) , 0*M(k)],
 *             [M(k)^2 , M(k)^2]]
 * The sequence a(n) is the n‑th entry when the matrix is read
 * row‑by‑row (row and column indices are 1‑based).  The first term
 * a(1) corresponds to the top‑left entry M(0)[1,1].
 *
 * Implementation notes:
 *   * We grow the matrix only as large as necessary to cover the requested
 *     row index.  For n up to 10000 the required matrix size is 256.
 *   * Squaring uses the lower‑triangular structure to avoid
 *     unnecessary work.  All arithmetic uses the seq_t type supplied by
 *     the harness; for 64‑bit builds it may overflow but the tests
 *     only check results that still fit in 64 bits.
 *   * No recursion is used; all matrices are stored in a flat array.
 */

#include <stdlib.h>
#include "../utils/square.h"

static seq_t *M = NULL;      /* current matrix (flat, row‑major)   */
static idx_t  Msize = 0;     /* current matrix size = 2^k (power of 2) */

/* Build a new matrix of size 'newsize', assuming the current matrix
 * M is already stored and of size Msize.  The new matrix is the
 * next step in the recursion.  After the call M points to the new
 * matrix and Msize is updated. */
static void build_matrix(idx_t newsize)
{
    seq_t *newM = (seq_t *)calloc((size_t)newsize * newsize, sizeof(seq_t));
    if (!newM) return; /* out of memory – caller will fail silently          */

    if (Msize == 0) {                     /* base case M(0) = [1] */
        newM[0] = (seq_t)1;
        M = newM;
        Msize = 1;
        return;
    }

    /* copy the old matrix into the top‑left block of the new matrix   */
    for (idx_t i = 0; i < Msize; ++i) {
        for (idx_t j = 0; j < Msize; ++j) {
            newM[i * newsize + j] = M[i * Msize + j];
        }
    }

    /* compute M(n)^2 into a temporary buffer */
    seq_t *sq = (seq_t *)calloc((size_t)Msize * Msize, sizeof(seq_t));
    if (!sq) {
        free(newM);
        return;
    }
    square(M, sq, Msize);

    /* copy square into the bottom‑left and bottom‑right blocks   */
    for (idx_t i = 0; i < Msize; ++i) {
        for (idx_t j = 0; j < Msize; ++j) {
            newM[(i + Msize) * newsize +       j] = sq[i * Msize + j];
            newM[(i + Msize) * newsize + (j + Msize)] = sq[i * Msize + j];
        }
    }

    free(M);
    free(sq);
    M = newM;
    Msize = newsize;
}

/* Ensure that the global matrix M has enough rows to cover 'need'.
 * It grows the matrix incrementally to the next power of two
 * that is at least 'need'. */
static void ensure_matrix(idx_t need)
{
    /* find the smallest power of two ≥ need   */
    idx_t pow2 = 1;
    while (pow2 < need) pow2 <<= 1;

    /* grow the matrix until it reaches that size    */
    while (Msize < pow2) {
        if (Msize == 0) {
            build_matrix(1);
        } else {
            build_matrix(Msize << 1);
        }
    }
}

/* Public sequence function.  Returns a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n <= 0) return (seq_t)0;        /* undefined for n ≤ 0 */

    /* find row r such that r*(r+1)/2 ≥ n   (1‑based) */
    idx_t r = 0;
    while (r * (r + 1) / 2 < n) ++r;
    idx_t prev = r * (r - 1) / 2;       /* entries before this row   */
    idx_t c = n - prev;                 /* column in this row (1‑based) */

    /* build a matrix large enough to contain row r          */
    ensure_matrix(r);

    /* convert to zero‑based indices                         */
    idx_t i = r - 1;
    idx_t j = c - 1;

    /* access the value from the matrix                      */
    return M[i * Msize + j];
}
