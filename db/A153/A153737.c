/*
 * A153737  A recursive triangular sequence with row sums (2*(n + 5)!):
 *          A(n,k)=A(n - 1, k - 1) + A(n - 1, k) + (n + 4)*(n + 3)*A(n - 2, k
 *          - 1).
 * https://oeis.org/A153737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k)=A(n-1,k-1)+A(n-1,k)+(n+4)*(n+3)*A(n-2,k-1) with preset
 *            rows 1..3: A(1,1)=2 A(2,1)=5040, A(2,2)=5040 A(3,1)=2,
 *            A(3,2)=80636, A(3,3)=2 a(n) is the n‑th element of the triangle
 *            read row by row
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 31, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Maximum number of rows we will generate (sufficient for n up to ~10000). */
#define MAX_ROWS 300

static seq_t *row_data[MAX_ROWS + 1] = {0}; /* row_data[n] points to row n (1‑based) */
static idx_t max_computed = 0;              /* highest row that has been computed */

/* Ensure that rows 1..n are computed. */
static void generate_rows_up_to(idx_t n) {
    if (n <= max_computed) return;

    for (idx_t r = max_computed + 1; r <= n; ++r) {
        /* Base rows */
        if (r == 1) {
            row_data[1] = (seq_t *)malloc(sizeof(seq_t));
            row_data[1][0] = (seq_t)2;
            continue;
        }
        if (r == 2) {
            row_data[2] = (seq_t *)malloc(2 * sizeof(seq_t));
            row_data[2][0] = row_data[2][1] = (seq_t)5040;
            continue;
        }
        if (r == 3) {
            row_data[3] = (seq_t *)malloc(3 * sizeof(seq_t));
            row_data[3][0] = (seq_t)2;
            row_data[3][1] = (seq_t)80636;
            row_data[3][2] = (seq_t)2;
            continue;
        }

        /* General recurrence for r >= 4 */
        generate_rows_up_to(r - 1);
        generate_rows_up_to(r - 2);

        size_t len = (size_t)r;
        row_data[r] = (seq_t *)malloc(len * sizeof(seq_t));

        seq_t coeff = (seq_t)(r + 4) * (seq_t)(r + 3); /* (n+4)*(n+3) */
        for (idx_t k = 1; k <= r; ++k) {
            seq_t v1 = 0, v2 = 0, v3 = 0;
            if (k > 1)          /* A(r-1,k-1) */
                v1 = row_data[r - 1][k - 2];
            if (k <= r - 1)      /* A(r-1,k) */
                v2 = row_data[r - 1][k - 1];
            if (k > 1 && k <= r - 1) /* A(r-2,k-1) */
                v3 = row_data[r - 2][k - 2];
            row_data[r][k - 1] = v1 + v2 + coeff * v3;
        }
    }
    max_computed = n;
}

/* Return the n-th term a(n) of OEIS sequence A153737 (offset 1). */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* safety check; sequence is 1‑based */

    /* Find the row that contains the n-th element. */
    idx_t remaining = n;
    idx_t row = 1;
    while (remaining > row) {
        remaining -= row;
        ++row;
    }
    /* Ensure that rows up to 'row' are available. */
    generate_rows_up_to(row);
    /* remaining is the 1‑based column index within that row. */
    return row_data[row][remaining - 1];
}
