/*
 * A091499  Triangle read by rows, such that T(n,k) equals the k-th term of
 *          the convolution of the two prior rows indexed by (n-k) and (k-1).
 * https://oeis.org/A091499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n, k) = Sum_{j=0..min(n-k, k)} T(n-k, j) * T(k-1, k-j-1), with
 *            T(n, 0) = 1, T(n, n) = 1 for n >= 0. The sequence a(n) is the
 *            triangle read by rows, i.e. a(n) = T(r, c) where r = max{m:
 *            m(m+1)/2 <= n}
 * Range:     u8 n <= 181, u16 n <= 579, u32 n <= 1652, u64 n <= 1652,
 *            u128 n <= 1652, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* ---------- internal storage ---------- */
static seq_t *data = NULL;          /* flattened triangle values */
static size_t data_cap = 0;         /* allocated capacity for data */
static size_t data_len = 0;         /* number of values stored */

/* row_start[row] = index in data of the first element of row `row` */
static idx_t *row_start = NULL;
static size_t rows_cap = 0;         /* allocated capacity for row_start */
static idx_t rows = 0;              /* number of rows already computed */

/* ---------- helper functions ---------- */

/* Ensure that `data` has room for at least `need` elements. */
static void ensure_data_cap(size_t need)
{
    if (need <= data_cap) return;
    size_t new_cap = data_cap ? data_cap * 3 / 2 : 64;
    while (new_cap < need) new_cap <<= 1;
    data = (seq_t *)realloc(data, new_cap * sizeof(seq_t));
    data_cap = new_cap;
}

/* Ensure that `row_start` has room for at least `need` rows. */
static void ensure_rows_cap(idx_t need)
{
    if (need <= (idx_t)rows_cap) return;
    size_t new_cap = rows_cap ? rows_cap * 3 / 2 : 32;
    while (new_cap < (size_t)need) new_cap <<= 1;
    row_start = (idx_t *)realloc(row_start, new_cap * sizeof(idx_t));
    rows_cap = new_cap;
}

/* Compute the next triangle row (index = `rows`). */
static void compute_next_row(void)
{
    idx_t r = rows;              /* current row index */
    idx_t len = r + 1;           /* number of columns in this row */

    /* Reserve space for the new row */
    ensure_rows_cap((idx_t)(rows + 1));
    row_start[rows] = (idx_t)data_len;      /* starting index of this row */

    size_t needed_data = data_len + (size_t)len;
    ensure_data_cap(needed_data);

    for (idx_t k = 0; k <= r; ++k) {
        seq_t val;
        if (k == 0 || k == r) {
            val = (seq_t)1;
        } else {
            /* j runs from 0 to min(r-k, k-1) */
            idx_t j_max = r - k;
            if (j_max > k - 1) j_max = k - 1;
            seq_t sum = (seq_t)0;
            for (idx_t j = 0; j <= j_max; ++j) {
                seq_t a = data[(size_t)row_start[r - k] + (size_t)j];
                seq_t b = data[(size_t)row_start[k - 1] + (size_t)(k - j - 1)];
                sum += a * b;
            }
            val = sum;
        }
        data[data_len++] = val;
    }
    rows++;
}

/* Compute rows until `data` contains at least `n+1` elements. */
static void ensure_rows_for_index(idx_t n)
{
    while (data_len <= n) compute_next_row();
}

/* Retrieve T(row, col) from the fully populated triangle. */
static seq_t get_T(idx_t row, idx_t col)
{
    return data[(size_t)row_start[row] + (size_t)col];
}

/* ---------- entry point ---------- */
seq_t a(idx_t n)
{
    /* Make sure we have computed all needed rows. */
    ensure_rows_for_index(n);

    /* Binary search to locate the row containing index n. */
    idx_t low = 0, high = rows - 1, r = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (row_start[mid] <= n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    idx_t k = n - row_start[r];
    return get_T(r, k);
}
