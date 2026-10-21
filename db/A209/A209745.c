/*
 * A209745  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209746; see the Formula section.
 * https://oeis.org/A209745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Triangle T(n,k) defined by * T(1,0) = 1 * T(2,0) = 1, T(2,1) = 2
 *            * T(n,k) = T(n-1,k) + 2*T(n-1,k-1) + T(n-2,k) + T(n-2,k-1) *
 *            (for k<0 or k≥n, the corresponding T is 0). * The output
 *            sequence a(n) is the flattened row‑major ordering of T. * For
 *            example: rows 1‑5 give a(1)=1, a(2)=1, a(3)=2, a(4)=...
 * Range:     u8 n <= 23, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Static storage for computed rows of the triangle */
static seq_t     *triangle_data = NULL;   /* contiguous storage of all T(n,k) values */
static size_t   *row_offset    = NULL;    /* row_offset[n] = index in triangle_data of row n, 1‑based */
static size_t     max_row       = 0;       /* highest row precomputed */
static size_t     data_len      = 0;       /* total number of elements allocated */

/* Ensure that rows 1 … up_to are computed and stored */
static void ensure_rows(size_t up_to)
{
    if (up_to <= max_row)
        return;

    /* Resize row_offset array */
    if (row_offset == NULL) {
        row_offset = (size_t*)malloc((up_to + 1) * sizeof(size_t));
    } else {
        row_offset = (size_t*)realloc(row_offset, (up_to + 1) * sizeof(size_t));
    }

    /* Compute total number of elements needed for rows 1…up_to */
    size_t new_data_len = up_to * (up_to + 1) / 2;
    if (triangle_data == NULL) {
        triangle_data = (seq_t*)malloc(new_data_len * sizeof(seq_t));
    } else {
        triangle_data = (seq_t*)realloc(triangle_data, new_data_len * sizeof(seq_t));
    }

    /* Fill missing rows */
    for (size_t n = max_row + 1; n <= up_to; ++n) {
        size_t offset_n = (n == 1) ? 0
                                 : row_offset[n - 1] + (n - 1);
        row_offset[n] = offset_n;

        for (size_t k = 0; k < n; ++k) {
            seq_t val = 0;

            if (n == 1 && k == 0)
                val = 1;
            else if (n == 2 && k == 0)
                val = 1;
            else if (n == 2 && k == 1)
                val = 2;
            else {
                seq_t a = 0, b = 0, c = 0, d = 0;

                /* T(n-1,k) */
                if (k < n - 1)
                    a = triangle_data[row_offset[n - 1] + k];

                /* 2 * T(n-1,k-1) */
                if (k > 0 && k - 1 < n - 1)
                    b = triangle_data[row_offset[n - 1] + (k - 1)];
                b = b * ((seq_t)2);

                /* T(n-2,k) */
                if (n >= 3 && k < n - 2)
                    c = triangle_data[row_offset[n - 2] + k];

                /* T(n-2,k-1) */
                if (n >= 3 && k > 0 && k - 1 < n - 2)
                    d = triangle_data[row_offset[n - 2] + (k - 1)];

                val = a + b + c + d;
            }

            triangle_data[offset_n + k] = val;
        }
    }

    max_row = up_to;
}

/* Main function requested by the harness */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined, but return 0 for safety */

    /* Find the row r such that  r*(r+1)/2 ≥ n  */
    size_t r = 1;
    idx_t cumulative = 1;
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }

    /* Column index (0‑based) within row r */
    idx_t prev_sum = (idx_t)(r - 1) * r / 2;   /* sum of lengths of previous rows */
    size_t col = (size_t)(n - prev_sum - 1);

    /* Ensure that row r is computed */
    ensure_rows(r);

    /* Return the requested entry */
    return triangle_data[row_offset[r] + col];
}
