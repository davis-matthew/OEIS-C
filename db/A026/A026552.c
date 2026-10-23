/*
 * A026552  Irregular triangular array T read by rows: T(n, 0) = T(n, 2*n) =
 *          1, T(n, 1) = T(n, 2*n-1) = floor(n/2 + 1), for even n >= 2, T(n,
 *          k) = T(n-1, k-2) + T(n-1, k-1) + T(n-1, k), otherwise T(n, k) =
 *          T(n-1, k-2) + T(n-1, k).
 * https://oeis.org/A026552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r, k) where n is the (0‑based) flattened index of * the
 *            triangular array T: T(0,0)=1; * T(1,0)=T(1,1)=T(1,2)=1; *
 *            T(n,0)=T(n,2n)=1; * T(n,1)=T(n,2n‑1)=⌊n/2⌋+1 (even n ≥ 2); *
 *            T(n,k) = T(n-1,k-2)+T(n-1,k-1)+T(n-1,k) (even n); * T(n,k) =
 *            T(n-1,k-2)+T(n-1,k) (odd n)
 * Range:     u8 n <= 70, u16 n <= 236, u32 n <= 805, u64 n <= 2855,
 *            u128 n <= 9348, big any n
 * Helpers:   get_prev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A026552 via the recurrence described in the entry. */
#include <stdlib.h>  /* malloc, realloc, free */
#include <stdint.h>  /* for size_t,uint64_t if needed */
#include "../utils/get_prev.h"

/* static storage for rows and cumulative row sizes */
static seq_t **rowp   = NULL;          /* array of pointers to rows   */
static idx_t *cum     = NULL;          /* cumulative counts up to row */
static size_t rows_len = 0;            /* number of rows stored       */
static size_t rows_cap = 0;            /* allocation capacity        */
static size_t cum_len   = 0;            /* number of cum entries      */
static size_t cum_cap   = 0;            /* allocation capacity        */

/* ensure capacity for row array */
static void ensure_rows_cap(size_t needed)
{
    if (needed <= rows_cap) return;
    size_t new_cap = (rows_cap == 0) ? 8 : rows_cap * 2;
    while (new_cap < needed) new_cap *= 2;
    seq_t **tmp = (seq_t **)realloc(rowp, new_cap * sizeof(seq_t *));
    rowp = tmp;
    rows_cap = new_cap;
}

/* ensure capacity for cumulative count array */
static void ensure_cum_cap(size_t needed)
{
    if (needed <= cum_cap) return;
    size_t new_cap = (cum_cap == 0) ? 8 : cum_cap * 2;
    while (new_cap < needed) new_cap *= 2;
    idx_t *tmp = (idx_t *)realloc(cum, new_cap * sizeof(idx_t));
    cum = tmp;
    cum_cap = new_cap;
}

/* build the next row (index r) using the recurrence and store it */
static void build_row(idx_t r)
{
    size_t len = 2 * r + 1;               /* length of row r           */
    seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
    if (!row) exit(1);                   /* out‑of‑memory; terminate  */

    if (r == 0) {                         /* base row 0                */
        row[0] = (seq_t)1;
    } else if (r == 1) {                  /* base row 1                */
        row[0] = row[1] = row[2] = (seq_t)1;
    } else {
        seq_t *prev     = rowp[r - 1];
        size_t prev_len = 2 * (r - 1) + 1;
        size_t i;

        for (i = 0; i < len; ++i) {
            /* boundary values */
            if (i == 0 || i == len - 1) {
                row[i] = (seq_t)1;
                continue;
            }
            /* special values for even indices */
            if ((r & 1ULL) == 0 && (i == 1 || i == len - 2)) {
                row[i] = (seq_t)(r / 2 + 1);
                continue;
            }
            /* recurrence */
            seq_t sum = (seq_t)0;
            if ((r & 1ULL) == 0) {          /* even n: 3‑term sum      */
                if (i >= 2) sum += get_prev(prev, i - 2, prev_len);
                sum += get_prev(prev, i - 1, prev_len);
                sum += get_prev(prev, i,     prev_len);
            } else {                        /* odd n:  2‑term sum      */
                if (i >= 2) sum += get_prev(prev, i - 2, prev_len);
                sum += get_prev(prev, i,     prev_len);
            }
            row[i] = sum;
        }
    }

    /* store the new row */
    ensure_rows_cap(rows_len + 1);
    rowp[rows_len++] = row;

    /* update cumulative count */
    idx_t new_cum = (rows_len == 1) ? (idx_t)len
                                    : cum[rows_len - 2] + (idx_t)len;
    ensure_cum_cap(rows_len);
    cum[rows_len - 1] = new_cum;
}

/* public function requested by the build */
seq_t a(idx_t n)
{
    /* build rows until the flattened index n is included */
    while (rows_len == 0 || cum[rows_len - 1] <= n) {
        if (rows_len == 0) {              /* initialise first two rows */
            build_row(0);
            build_row(1);
        } else {
            build_row(rows_len);          /* next row                */
        }
    }

    /* binary search for the first cum value > n */
    size_t lo = 0, hi = rows_len;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (cum[mid] <= n) lo = mid + 1;
        else hi = mid;
    }
    size_t r = lo;                        /* row index containing n  */
    idx_t start = (r == 0) ? (idx_t)0 : cum[r - 1];
    size_t k = (size_t)(n - start);       /* column within the row   */
    return rowp[r][k];
}
