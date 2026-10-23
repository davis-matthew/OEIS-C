/*
 * A153648  Triangle T(n, k) = T(n-1, k) + T(n-1, k-1) + j*prime(j)*T(n-2,
 *          k-1) with j=3, read by rows.
 * https://oeis.org/A153648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A153648 – Triangle T(n,k) = T(n-1,k) + T(n-1,k-1) +
 *            3*prime(3)*T(n-2,k-1), j=3; read by rows. a(n) is the n‑th entry
 *            of the flattened triangle
 * Range:     u8 n <= 12, u16 n <= 31, u32 n <= 112, u64 n <= 420,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Global storage for computed rows of the triangle.
   triangle_rows[i] points to an array of seq_t of length i+1 (indices 1..i). */
static seq_t **triangle_rows = NULL;  /* array of row pointers */
static size_t triangle_max_row = 0;   /* highest row that has been computed */

/* Ensure that rows up to 'max_row' are allocated and filled.
   This function is called the first time a new row is needed. */
static void ensure_rows(size_t max_row)
{
    if (max_row <= triangle_max_row)
        return;

    /* Resize the array of row pointers to hold indices 0..max_row. */
    size_t new_cap = max_row + 1;          /* index 0 is unused */
    if (triangle_rows == NULL) {
        triangle_rows = (seq_t**)malloc(new_cap * sizeof(seq_t*));
    } else {
        triangle_rows = (seq_t**)realloc(triangle_rows, new_cap * sizeof(seq_t*));
    }

    const seq_t PRIME    = 5;          /* prime(3) */
    const seq_t J_PRIME  = 15;         /* 3 * prime(3) */

    /* Allocate memory for any new rows that did not exist before. */
    for (size_t i = triangle_max_row + 1; i <= max_row; ++i) {
        triangle_rows[i] = (seq_t*)malloc((i + 1) * sizeof(seq_t));
    }

    /* Base rows 1, 2, 3.  We set them only if they haven’t been set already. */
    if (triangle_max_row < 1 && max_row >= 1) {
        triangle_rows[1][1] = 2;
    }

    if (triangle_max_row < 2 && max_row >= 2) {
        triangle_rows[2][1] = PRIME;
        triangle_rows[2][2] = PRIME;
    }

    if (triangle_max_row < 3 && max_row >= 3) {
        triangle_rows[3][1] = 2;
        triangle_rows[3][3] = 2;
        triangle_rows[3][2] = 2 * PRIME * PRIME - 4;   /* 46 */
    }

    /* Compute all rows from the first new row >= 4 up to max_row. */
    for (size_t n = (triangle_max_row >= 4 ? triangle_max_row + 1 : 4); n <= max_row; ++n) {
        seq_t *prev  = triangle_rows[n - 1];
        seq_t *prev2 = triangle_rows[n - 2];
        seq_t *curr  = triangle_rows[n];

        curr[1] = 2;
        curr[n] = 2;

        for (size_t k = 2; k < n; ++k) {
            curr[k] = prev[k] + prev[k - 1] + J_PRIME * prev2[k - 1];
        }
    }

    triangle_max_row = max_row;
}

/* Return the n-th term of the sequence (a(1) = 2). */
seq_t a(idx_t n)
{
    if (n <= 0)
        return 0;              /* undefined for non‑positive indices */

    /* Find the row 'r' such that the cumulative count of terms up to r
       is at least n.  We use the fact that the number of terms up to row r
       equals r*(r+1)/2. */
    size_t r = 1;
    idx_t cumulative = 1;          /* sum of lengths 1..r */
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }

    /* Column index k within row r. */
    idx_t prev_cumulative = cumulative - r;  /* sum of lengths up to row r-1 */
    size_t k = (size_t)(n - prev_cumulative);

    /* Make sure the required rows have been computed. */
    ensure_rows(r);

    return triangle_rows[r][k];
}
