/*
 * A152613  Triangle read by rows: T(n,k) = 3*T(n-1,k-1) + 3*T(n-1,k) -
 *          2*T(n-2,k-1) with T(n,0) = T(n,n) = 1.
 * https://oeis.org/A152613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 47, u32 n <= 125, u64 n <= 417,
 *            u128 n <= 1325, big any n
 * Helpers:   row_col_from_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows:  T(n,k) = 3*T(n-1,k-1) + 3*T(n-1,k) - 2*T(n-2,k-1)
 * with T(n,0)=T(n,n)=1.
 * a(n) is the triangle enumerated row by row (row 0: 1, row 1: 1 1, row 2: 1 4 1, …).
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/row_col_from_n.h"

/* Global storage for the triangle in compact triangular form. */
static seq_t *triangle = NULL;
static idx_t  max_row   = 0;   /* highest row fully computed, triangle[0] is row 0 */

/* Compute all rows up to 'up_to'.  The function is thread‑unsafe but
 * the problem statement assumes single threaded use. */
static void
ensure_rows(idx_t up_to)
{
    if (triangle == NULL) {
        /* initial allocation for row 0 */
        size_t total = 1;
        triangle = (seq_t *)malloc(total * sizeof(seq_t));
        if (!triangle) abort();
        triangle[0] = 1;
        max_row = 0;
    }

    if (up_to <= max_row) return;

    /* new total number of elements after adding rows */
    size_t new_total = (size_t)(up_to + 1) * (up_to + 2) / 2;
    seq_t *new_tri = (seq_t *)malloc(new_total * sizeof(seq_t));
    if (!new_tri) abort();

    /* copy previous data */
    memcpy(new_tri, triangle, (size_t)(max_row + 1) * (max_row + 2) / 2 * sizeof(seq_t));
    free(triangle);
    triangle = new_tri;

    /* compute missing rows */
    for (idx_t r = max_row + 1; r <= up_to; ++r) {
        size_t offset       = (size_t)r * (r + 1) / 2;   /* start of row r */
        size_t prev_offset  = (size_t)(r - 1) * r      / 2;
        size_t prev2_offset = (size_t)(r - 2) * (r - 1) / 2;

        triangle[offset]          = 1;            /* left boundary */
        triangle[offset + r]      = 1;            /* right boundary */

        if (r >= 2) {
            for (idx_t k = 1; k < r; ++k) {
                seq_t a = triangle[prev_offset      + k - 1];
                seq_t b = triangle[prev_offset      + k];
                seq_t c = triangle[prev2_offset     + k - 1];
                triangle[offset + k] = 3 * a + 3 * b - 2 * c;
            }
        }
    }

    max_row = up_to;
}

/* Public sequence function. */
seq_t
a(idx_t n)
{
    idx_t r, k;
    row_col_from_n(n, &r, &k);
    ensure_rows(r);
    size_t offset = (size_t)r * (r + 1) / 2;
    return triangle[offset + k];
}
