/*
 * A089177  Triangle read by rows: T(n,k) (n >= 0, 0 <= k <=
 *          1+log_2(floor(n))) giving number of non-squashing partitions of n
 *          into k parts.
 * https://oeis.org/A089177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, look, easy
 * Formula:   not stated
 * Range:     u8 n <= 115, u16 n <= 653, u32 n <= 6089, u64 n <= 9873,
 *            u128 n <= 9873, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A089177:  Triangle read by rows
    T(0,0) = 1
    For n > 0:  T(n,0) = 1
               T(n,k) = T(n-1,k) + T(⌊n/2⌋,k-1)   (k > 0)
    a(n) is the concatenation of all rows of T in increasing
    row order.  Offset 0.  */
#include <stdlib.h>
#include <stdint.h>

static idx_t *cum_start = NULL;     /* start index of each row */
static size_t *row_len  = NULL;     /* lengths of rows */
static seq_t **rows     = NULL;     /* pointers to row data */
static size_t rows_cnt  = 0;        /* number of rows computed */
static size_t rows_cap  = 0;        /* allocated capacity for rows arrays */
static idx_t  next_start = 0;       /* start index of the next row */

/*--- Utility: floor(log2) for unsigned 64‑bit integers --------------------*/
static inline idx_t floor_log2_uint64(idx_t v)
{
    idx_t r = 0;
    while (v >>= 1) ++r;
    return r;
}

/*--- Allocation helper for seq_t arrays ---------------------------------*/
static seq_t *alloc_seq_array(size_t n)
{
#ifdef __cplusplus
    return new seq_t[n];
#else
    seq_t *p = (seq_t *)malloc(n * sizeof(seq_t));
    if (!p) abort();
    return p;
#endif
}

/*--- Resize storage for row pointers, lengths and cumulative starts -------*/
static void ensure_rows_capacity(size_t needed)
{
    if (needed <= rows_cap) return;
    size_t new_cap = rows_cap ? rows_cap * 2 : 4;
    while (new_cap < needed) new_cap <<= 1;

    seq_t **new_rows = NULL;
    size_t *new_len  = NULL;
    idx_t *new_start = NULL;

    new_rows  = (seq_t **)realloc(rows,  new_cap * sizeof(seq_t *));
    new_len   = (size_t *)realloc(row_len, new_cap * sizeof(size_t));
    new_start = (idx_t *)realloc(cum_start, new_cap * sizeof(idx_t));

    if (!new_rows || !new_len || !new_start) abort();

    rows     = new_rows;
    row_len  = new_len;
    cum_start= new_start;
    rows_cap = new_cap;
}

/*--- Build the next row (row index = rows_cnt) ---------------------------*/
static void build_next_row(void)
{
    idx_t r = rows_cnt;
    ensure_rows_capacity(rows_cnt + 1);

    size_t len_cur = (r == 0 ? 1 : floor_log2_uint64(r) + 2);
    seq_t *arr = alloc_seq_array(len_cur);
    if (!arr) abort();

    arr[0] = (seq_t)1;                      /* T(r,0) = 1 */

    if (r > 0) {
        /* previous row is r-1 */
        seq_t *prev    = rows[r-1];
        size_t len_prev = row_len[r-1];

        /* parent row (floor(r/2)) */
        idx_t parent = r >> 1;
        seq_t *par    = rows[parent];
        size_t len_par = row_len[parent];

        for (size_t k = 1; k < len_cur; ++k) {
            seq_t v1 = (k < len_prev) ? prev[k] : (seq_t)0;
            seq_t v2 = (k-1 < len_par) ? par[k-1] : (seq_t)0;
            arr[k] = v1 + v2;
        }
    }

    cum_start[rows_cnt] = next_start;      /* record start of this row */
    rows[rows_cnt]      = arr;
    row_len[rows_cnt]   = len_cur;
    next_start          += (idx_t)len_cur; /* start of next row */
    ++rows_cnt;
}

/*--- Binary search for the row containing index n ---------------------*/
static size_t find_row(idx_t n)
{
    size_t low = 0, high = rows_cnt;      /* [low, high) interval */
    while (low + 1 < high) {
        size_t mid = (low + high) >> 1;
        if (n < cum_start[mid])
            high = mid;
        else
            low = mid;
    }
    return low;
}

/*--- Public function ---------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Ensure enough rows have been built to cover index n */
    while (next_start <= n) {
        build_next_row();
    }

    size_t row = find_row(n);
    size_t col = (size_t)(n - cum_start[row]);

    return rows[row][col];
}
