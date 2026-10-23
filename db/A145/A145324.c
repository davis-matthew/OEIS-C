/*
 * A145324  Triangle read by rows: coefficients of 1; 1(X+2); 1(X+2)(X+3);
 *          1(X+2)(X+3)(X+4); ....
 * https://oeis.org/A145324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the triangle read by rows with T(1,1)=1;
 *            T(n,k)=T(n-1,k)+n*T(n-1,k-1)
 * Range:     u8 n <= 18, u16 n <= 34, u32 n <= 87, u64 n <= 225,
 *            u128 n <= 589, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t **triangle = NULL; /* rows of the triangle, triangle[0] unused */
static idx_t  max_row   = 0;     /* largest row that has been built */

/* Ensure that rows 1..r exist in the global array. */
static void
ensure_rows(idx_t r)
{
    if (r <= max_row)
        return;           /* already built */

    /* Allocate space for the pointer array of rows. */
    seq_t **tmp = (seq_t**)realloc(triangle,
                                  (size_t)(r + 1) * sizeof(seq_t*));
    if (!tmp)
        return;           /* OOM – caller will treat the value as 0 */

    triangle = tmp;
    triangle[0] = NULL;    /* dummy */

    /* Build each new row one by one. */
    for (idx_t i = max_row + 1; i <= r; ++i) {
        /* allocate the row; index 0 is unused but filled with 0 */
        seq_t *row = (seq_t*)calloc((size_t)(i + 1), sizeof(seq_t));
        if (!row)
            return;       /* OOM */

        triangle[i] = row;
        row[0] = 0;      /* not used by the recurrence */

        if (i == 1) {    /* base case */
            row[1] = 1;
        } else {
            seq_t *prev = triangle[i - 1];
            for (idx_t k = 1; k <= i; ++k) {
                seq_t a = (k <= i - 1) ? prev[k] : 0; /* T(i-1,k) */
                seq_t b = prev[k - 1];                /* T(i-1,k-1) */
                row[k] = a + (seq_t)i * b;            /* T(i,k) */
            }
        }
    }
    max_row = r;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* off‑offset */

    /* Find the row r such that triangular number r(r+1)/2 >= n. */
    idx_t r = 1;
    idx_t cum = 0;
    while (cum + r < n) {
        cum += r;
        ++r;
    }
    idx_t c = n - cum;           /* column within row, 1‑based */

    ensure_rows(r);
    if (!triangle || !triangle[r] || c == 0 || c > r)
        return 0;               /* OOM or invalid index */

    return triangle[r][c];
}
