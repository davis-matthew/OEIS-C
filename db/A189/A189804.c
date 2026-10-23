/*
 * A189804  Triangle read by rows: T(n,k) is the number of compositions of set
 *          {1, 2, ..., k} into exactly n blocks, each of size 1, 2 or 3 (n >=
 *          0, 0 <= k <= 3*n).
 * https://oeis.org/A189804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 30, u32 n <= 64, u64 n <= 139,
 *            u128 n <= 324, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula for a(n): the triangle read by rows with rows given by
   T(0,0)=1 and for n>0, 0 ≤ k ≤ 3n
   T(n,k) = k*T(n-1,k-1)
         + (k*(k-1)/2)*T(n-1,k-2)
         + (k*(k-1)*(k-2)/6)*T(n-1,k-3)            */
#include <stdlib.h>
#include <stdbool.h>

/* integer square root: floor(sqrt(x)) for idx_t */
static inline idx_t isqrt(idx_t x) {
    idx_t r = 0;
    idx_t bit = (idx_t)1 << 62;   /* highest power of four ≤ 2^64 */
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* ----- cache of computed rows ----- */
static seq_t **rows = NULL;   /* array of pointers to rows */
static size_t rows_cap = 0;   /* capacity of rows array */
static size_t max_row = 0;    /* highest row that has been computed */

/* ensure rows 0..r are computed and cached */
static void ensure_rows(idx_t r) {
    /* first time: allocate rows array and row 0 */
    if (rows_cap == 0) {
        rows_cap = 1;
        rows = (seq_t**)malloc(rows_cap * sizeof(seq_t*));
        /* row 0 has length 1 */
        seq_t *row0 = (seq_t*)malloc(1 * sizeof(seq_t));
#ifdef __cplusplus
        new(&row0[0]) seq_t();
#endif
        row0[0] = (seq_t)1;
        rows[0] = row0;
        max_row = 0;
    }
    if (r <= max_row) return;

    /* extend rows array if needed */
    if (r + 1 > (idx_t)rows_cap) {
        size_t new_cap = rows_cap * 2;
        if (new_cap < (size_t)(r + 1)) new_cap = r + 1;
        seq_t **tmp = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
        rows = tmp;
        rows_cap = new_cap;
    }

    /* compute new rows */
    for (idx_t i = max_row + 1; i <= r; ++i) {
        size_t len = 3 * (size_t)i + 1;
        seq_t *row = (seq_t*)malloc(len * sizeof(seq_t));
#ifdef __cplusplus
        for (size_t j = 0; j < len; ++j)
            new(&row[j]) seq_t();
#endif
        if (i == 0) {          /* already done above, but keep for safety */
            row[0] = (seq_t)1;
        } else {
            idx_t len_prev = 3 * (i - 1) + 1;
            for (size_t k = 0; k < len; ++k) {
                seq_t val = (seq_t)0;
                if (k >= 1 && k - 1 < len_prev) {
                    val += (seq_t)k * rows[i - 1][k - 1];
                }
                if (k >= 2 && k - 2 < len_prev) {
                    seq_t coeff = (seq_t)k * (seq_t)(k - 1) / 2;
                    val += coeff * rows[i - 1][k - 2];
                }
                if (k >= 3 && k - 3 < len_prev) {
                    seq_t coeff = (seq_t)k * (seq_t)(k - 1) *
                                  (seq_t)(k - 2) / 6;
                    val += coeff * rows[i - 1][k - 3];
                }
                row[k] = val;
            }
        }
        rows[i] = row;
    }
    max_row = (size_t)r;
}

/* public sequence function */
seq_t a(idx_t n) {
    /* find row r such that S(r) = (3r^2 - r)/2 ≤ n < S(r+1)   */
    idx_t r = (1 + isqrt(1 + (idx_t)24 * n)) / 6;
    idx_t offset = r + (idx_t)3 * r * (r - 1) / 2; /* S(r) */
    idx_t k = n - offset;   /* position within the row */

    ensure_rows(r);         /* make sure T(r,·) exists */

    return rows[r][k];
}
