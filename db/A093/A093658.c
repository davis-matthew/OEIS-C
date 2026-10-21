/*
 * A093658  Lower triangular matrix, read by rows, defined as the convergent
 *          of the concatenation of matrices using the iteration: M(n+1) =
 *          [[M(n),0*M(n)],[M(n)^2,M(n)]], with M(0) = [1].
 * https://oeis.org/A093658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = M(r-1)[r-1][c-1] where r is the row such that (r-1)*r/2 <
 *            n ≤ r*(r+1)/2 and c = n - (r-1)*r/2. M(0) = [1]; M(k+1) =
 *            [[M(k),0],[M(k)²,M(k)]] (block 2×2)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: no external helpers are used – we employ the standard library. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Static storage for the matrix and its current size (a power of two). */
static seq_t *matrix = NULL;
static size_t curr_size = 0;

/* Expand the stored matrix so that its dimension is at least `need`. */
static void ensure_size(size_t need)
{
    while (curr_size < need) {
        size_t old_size = curr_size;
        curr_size = (curr_size == 0) ? 1 : (curr_size << 1);

        /* Allocate a new N×N matrix. */
        seq_t *newM = (seq_t *)malloc(curr_size * curr_size * sizeof(seq_t));
        if (!newM) {
            /* In a real build we would handle allocation failure. */
            abort();
        }

        /* Initialize all entries to zero. */
        for (size_t i = 0; i < curr_size * curr_size; ++i)
            newM[i] = 0;

        if (old_size == 0) {
            /* Base matrix M(0) = [1] */
            newM[0] = 1;
        } else {
            /* Copy top-left and bottom-right blocks (both equal to old matrix). */
            for (size_t i = 0; i < old_size; ++i) {
                for (size_t j = 0; j < old_size; ++j) {
                    size_t idx_new = i * curr_size + j;
                    size_t idx_old = i * old_size + j;
                    newM[idx_new] = matrix[idx_old];
                    newM[(i + old_size) * curr_size + (j + old_size)] = matrix[idx_old];
                }
            }

            /* Bottom-left block = M(old)². */
            for (size_t i = 0; i < old_size; ++i) {
                for (size_t j = 0; j < old_size; ++j) {
                    seq_t sum = 0;
                    for (size_t k = 0; k < old_size; ++k)
                        sum += matrix[i * old_size + k] * matrix[k * old_size + j];
                    newM[(i + old_size) * curr_size + j] = sum;
                }
            }
        }

        /* Free the previous matrix and replace it. */
        free(matrix);
        matrix = newM;
    }
}

/* Return the n-th term of the sequence, indexed from 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset 1 – undefined below 1 */

    /* Find the row r such that (r-1)*r/2 < n ≤ r*(r+1)/2. */
    idx_t r = 1;
    while (((r * (r + 1)) / 2) < n) ++r;

    /* Column number (1‑based) within the row. */
    idx_t c = n - ((r * (r - 1)) / 2);

    /* Ensure the matrix has at least `r` rows. */
    ensure_size((size_t)r);

    /* Return the matrix entry at zero‑based position (r-1, c-1). */
    return matrix[(r - 1) * curr_size + (c - 1)];
}
