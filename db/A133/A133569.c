/*
 * A133569  A133566 * A007318 as infinite lower triangular matrices.
 * https://oeis.org/A133569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element of the flattened lower triangular
 *            matrix T(n,k) defined by T(n,k) = T(n-2,k) + 2*T(n-2,k-1) +
 *            T(n-2,k-2) for n ≥ 3, with base rows T(0,0)=1; T(1,0)=T(1,1)=1;
 *            T(2,0)=2; T(2,1)=3; T(2,2)=1; (T(n,k)=0 for k<0 or k>n). The
 *            sequence a(n) = T(r,k) where r is the row containing...
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t **rows = NULL;      /* array of pointers to rows          */
static size_t rows_alloc = 0;    /* allocated length of rows[]         */
static size_t rows_count = 0;    /* number of computed rows (row 0 is row 0, …) */

static void ensure_rows_array_size(size_t needed)
{
    if (rows_alloc <= needed) {
        size_t new_alloc = rows_alloc ? rows_alloc * 2 : 2;
        while (new_alloc <= needed) new_alloc <<= 1;
        rows = (seq_t**)realloc(rows, new_alloc * sizeof(seq_t *));
        rows_alloc = new_alloc;
    }
}

static void compute_rows_upto(size_t max_row)
{
    if (max_row < rows_count) return;            /* already done */

    ensure_rows_array_size(max_row);

    for (size_t r = rows_count; r <= max_row; ++r) {
        rows[r] = (seq_t*)malloc((r + 1) * sizeof(seq_t));
        if (!rows[r]) { exit(1); }               /* allocation failure */

        if (r == 0) {
            rows[r][0] = (seq_t)1;
        } else if (r == 1) {
            rows[r][0] = rows[r][1] = (seq_t)1;
        } else if (r == 2) {                     /* base row from OEIS */
            rows[r][0] = (seq_t)2;
            rows[r][1] = (seq_t)3;
            rows[r][2] = (seq_t)1;
        } else {                                 /* recurrence for r ≥ 3 */
            size_t prev = r - 2;
            size_t len_prev = prev + 1;
            for (size_t k = 0; k <= r; ++k) {
                seq_t val = (seq_t)0;
                if (k < len_prev)                 /* T(r-2,k)   */
                    val += rows[prev][k];
                if (k >= 1 && k - 1 < len_prev)   /* 2*T(r-2,k-1)  */
                    val += (seq_t)2 * rows[prev][k - 1];
                if (k >= 2 && k - 2 < len_prev)   /* T(r-2,k-2)    */
                    val += rows[prev][k - 2];
                rows[r][k] = val;
            }
        }
    }
    rows_count = max_row + 1;
}

seq_t a(idx_t n)   /* n is 1‑based */
{
    /* Find row r such that the cumulative term count up to row r
       (inclusive) is at least n.  The cumulated count of rows 0…r
       equals (r+1)(r+2)/2. */
    size_t r = 0;
    for (;;) {
        size_t cum = (r + 1) * (r + 2) / 2;
        if (cum >= (size_t)n) break;
        ++r;
    }

    /* Start index of this row (1‑based). */
    size_t row_start = r * (r + 1) / 2 + 1;
    size_t k = (size_t)n - row_start;   /* 0‑based column index */

    /* Ensure the needed rows are computed. */
    if (r >= rows_count)
        compute_rows_upto(r);

    return rows[r][k];
}
