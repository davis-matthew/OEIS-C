/*
 * A140998  Triangle G(n, k), read by rows, for 0 <= k <= n, where G(n, 0) =
 *          G(n+1, n+1) = 1, G(n+2, n+1) = 2, and G(n+3, m) = G(n+1, m-1) +
 *          G(n+1, m) + G(n+2, m) for n >= 0 and m = 1..n+1.
 * https://oeis.org/A140998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = element at row floor((sqrt(8*n+1)-1)/2), column n -
 *            row*(row+1)/2; where the triangle G(n, k) is defined by G(n, 0)
 *            = G(n, n) = 1, G(n, n-1) = 2 for n ≥ 2, and for n ≥ 3, G(n, k) =
 *            G(n-2, k-1) + G(n-2, k) + G(n-1, k) for 1 ≤ k ≤ n-2
 * Range:     u8 n <= 56, u16 n <= 174, u32 n <= 634, u64 n <= 2288,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>

/* external integer types provided by the build */
extern seq_t a(idx_t n);

/* static cache for computed rows of the triangle */
static seq_t **triangle = NULL;          /* array of row pointers */
static idx_t  max_rows = 0;              /* number of rows currently allocated */

/* ensure that rows 0 .. up_to are computed and stored in triangle */
static void ensure_rows(idx_t up_to)
{
    /* grow the array of row pointers if necessary */
    if (triangle == NULL) {
        triangle = (seq_t**)malloc((up_to + 1) * sizeof(seq_t*));
    } else if ((size_t)(up_to + 1) > max_rows) {
        triangle = (seq_t**)realloc(triangle,
                                    (up_to + 1) * sizeof(seq_t*));
    }

    /* compute new rows incrementally */
    for (idx_t r = max_rows; r <= up_to; ++r) {
        seq_t *row = (seq_t*)malloc((r + 1) * sizeof(seq_t));
        if (!row) exit(1);              /* out of memory */

        /* first and last entries */
        row[0] = 1;
        if (r == 0) {
            triangle[r] = row;
            continue;
        }
        row[r] = 1;

        /* second‑last entry for r ≥ 2 */
        if (r >= 2)
            row[r - 1] = 2;

        /* interior entries for r ≥ 3 */
        if (r >= 3) {
            for (idx_t k = 1; k <= r - 2; ++k) {
                seq_t a = triangle[r - 2][k - 1];
                seq_t b = triangle[r - 2][k];
                seq_t c = triangle[r - 1][k];
                row[k] = a + b + c;
            }
        }

        triangle[r] = row;
    }

    max_rows = up_to + 1;
}

/* compute the n-th term of the sequence */
seq_t a(idx_t n)
{
    /* locate the row containing index n */
    idx_t row = (idx_t)((sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0);

    /* adjust in case the floating estimate was off */
    while ((unsigned __int128)(row + 1) * (row + 2) / 2 <= n) ++row;
    while ((unsigned __int128)row * (row + 1) / 2 > n) --row;

    /* compute the triangular offset of the row */
    unsigned __int128 tri = (unsigned __int128)row * (row + 1) / 2;
    idx_t col = n - (idx_t)tri;

    /* ensure all necessary rows are computed */
    ensure_rows(row);

    return triangle[row][col];
}
