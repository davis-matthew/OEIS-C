/*
 * A080249  Stirling-like number triangle defined by the sequence
 *          A000292=C(n+3,3).
 * https://oeis.org/A080249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = flatten(T(n,k)) where T(0,0)=1; T(n,0)=T(n,n)=1;
 *            T(n,k)=T(n-1,k-1)+C(k+3,3)*T(n-1,k) (1≤k≤n-1),
 *            C(k+3,3)=(k+1)(k+2)(k+3)/6
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static storage for the triangle rows. */
static seq_t **rows      = NULL;   /* array of pointers to rows */
static size_t  rows_cap  = 0;      /* current capacity of rows array */
static size_t  rows_cnt  = 0;      /* number of rows actually stored */

/* Ensure that the rows array can hold at least new_cap pointers. */
static void
ensure_rows_cap(size_t new_cap)
{
    if (new_cap <= rows_cap)
        return;

    size_t new_cap_small = rows_cap ? rows_cap * 2 : 4;
    if (new_cap_small < new_cap)
        new_cap_small = new_cap;

    seq_t **new_rows = (seq_t **)realloc(rows, new_cap_small * sizeof(seq_t *));
    if (!new_rows)
        abort();
    rows       = new_rows;
    rows_cap   = new_cap_small;
}

/* Build all rows of the triangle up to max_row (inclusive). */
static void
ensure_rows_up_to(idx_t max_row)
{
    if (max_row + 1 <= rows_cnt)
        return;   /* already have them */

    ensure_rows_cap(max_row + 1);

    for (idx_t r = rows_cnt; r <= max_row; ++r) {
        size_t len = (size_t)(r + 1);
        seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
        if (!row)
            abort();

        row[0] = 1;          /* first element is always 1 */
        if (r > 0) {
            for (idx_t k = 1; k < len - 1; ++k) {
                /* C(k+3,3) = (k+1)(k+2)(k+3)/6 */
                seq_t coef = ((seq_t)(k + 1) * (k + 2) * (k + 3)) / 6;
                seq_t left = rows[r - 1][k - 1];
                seq_t right = rows[r - 1][k];
                row[k] = left + coef * right;
            }
            row[len - 1] = 1;   /* last element is always 1 */
        }
        rows[r] = row;
    }
    rows_cnt = max_row + 1;
}

/* Find the row and column (r,k) in the triangle for the flattened index n. */
static void
row_col_from_index(idx_t n, idx_t *r_out, idx_t *k_out)
{
    /* Binary search for r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t low = 0, high = n, r = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri <= n) {
            r = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    *r_out = r;

    __uint128_t row_start = (__uint128_t)r * (r + 1) / 2;
    *k_out = (idx_t)(n - (idx_t)row_start);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t r, k;
    row_col_from_index(n, &r, &k);
    ensure_rows_up_to(r);
    return rows[r][k];
}
