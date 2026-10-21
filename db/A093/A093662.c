/*
 * A093662  Lower triangular matrix, read by rows, defined as the convergent
 *          of the concatenation of matrices using the iteration: M(n+1) =
 *          [[M(n),0*M(n)],[M(n),M(n)^2]], with M(0) = [1].
 * https://oeis.org/A093662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A093662 – Lower‑triangular matrix read by rows.
 *
 * The infinite lower‑triangular matrix M is built iteratively from
 *
 *     M(0) = [1]
 *     M(k+1) = [ M(k)      0      ;
 *                M(k)    M(k)^2 ]      (ordinary matrix product)
 *
 * The sequence a(n) is obtained by listing the entries of the matrix
 * in row‑major order, skipping the zero entries that lie above the
 * diagonal.  For example a(1)=1, a(2)=1, a(3)=1, a(4)=1, a(5)=0, …
 *
 * This implementation keeps the matrix in a contiguous one‑dimensional
 * array.  The matrix is enlarged only when a larger row is required,
 * and the expansion doubles its size so that the number of re‑allocations
 * is logarithmic in the requested index.
 *
 * The implementation uses only the integer types defined by the build:
 *   seq_t – the value type (uint8_t, uint16_t, …, unsigned __int128, or a
 *            arbitrary‑precision type with the usual arithmetic operators)
 *   idx_t – an unsigned integer type for indices (uint64_t, …).
 */

#include <stdlib.h>

/* Forward declaration of global state */
static seq_t *matrix = NULL;      /* pointer to the current matrix          */
static size_t curSize = 0;        /* current matrix dimension (equal to rows) */

/* Compute the minimal row r (1‑based) that contains index n  */
static idx_t compute_row(idx_t n)
{
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;
    return r;
}

/* Grow the matrix until it has at least rows_needed rows */
static void build_until(idx_t rows_needed)
{
    /* Initialisation: matrix of size 1×1 with entry 1 */
    if (curSize == 0) {
        matrix = (seq_t *)malloc(sizeof(seq_t));
        if (!matrix) abort();          /* out of memory */
        matrix[0] = (seq_t)1;
        curSize = 1;
        return;
    }

    /* Loop until the matrix is large enough */
    while ((size_t)rows_needed > curSize) {
        size_t oldSize = curSize;
        size_t newSize = oldSize * 2;
        /* Allocate new matrix and initialise to zero */
        seq_t *newMat = (seq_t *)calloc(newSize * newSize, sizeof(seq_t));
        if (!newMat) abort();

        /* Copy top‑left and bottom‑left blocks from the old matrix */
        for (size_t i = 0; i < oldSize; ++i) {
            for (size_t j = 0; j < oldSize; ++j) {
                seq_t val = matrix[i * oldSize + j];
                /* top‑left */
                newMat[i * newSize + j] = val;
                /* bottom‑left */
                newMat[(i + oldSize) * newSize + j] = val;
            }
        }

        /* Compute bottom‑right block = oldMatrix × oldMatrix */
        for (size_t i = 0; i < oldSize; ++i) {
            for (size_t j = 0; j < oldSize; ++j) {
                seq_t sum = (seq_t)0;
                for (size_t k = 0; k < oldSize; ++k) {
                    sum += matrix[i * oldSize + k] * matrix[k * oldSize + j];
                }
                newMat[(i + oldSize) * newSize + (j + oldSize)] = sum;
            }
        }

        /* Replace the old matrix */
        free(matrix);
        matrix = newMat;
        curSize = newSize;
    }
}

/* Public function implementing A093662 */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* indices below the offset are unused */

    /* Determine the row that contains index n (1‑based) */
    idx_t row = compute_row(n);          /* row >= 1 */
    /* Ensure the matrix is large enough */
    build_until(row);

    /* Convert to 0‑based indices for array access */
    idx_t row0 = row - 1;
    idx_t col0 = n - ((row - 1) * row / 2) - 1;   /* column (0‑based) */

    return matrix[row0 * curSize + col0];
}
