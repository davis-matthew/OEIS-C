/*
 * A096811  Triangle, read by rows, such that T(n,k) equals the k-th term of
 *          the convolution of the two prior rows indexed by (n-k) and (k-2).
 * https://oeis.org/A096811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 390, u16 n <= 1200, u32 n <= 4141, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   pref
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:   T(n,k) = 1          if k==0 or k==1 or k==n
 *                T(n,k) = Σ_{j=1..min(n-k,k-1)} T(n-k,j) * T(k-2,k-j-1)
 * a(n) is obtained by reading the triangle row‑wise.
 * a(0) = 1, a(1) = 1, …                                       */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/pref.h"

static seq_t **rows = NULL;      /* array of pointers to rows           */
static size_t   rows_used = 0;   /* number of rows already stored       */
static size_t   rows_cap  = 0;   /* capacity of the rows array          */

/* ensure that the rows array can hold at least n rows                  */
static void ensure_rows_cap(size_t n)
{
    if (rows_cap >= n) return;
    size_t new_cap = rows_cap ? rows_cap << 1 : 16;
    while (new_cap < n) new_cap <<= 1;
    rows = (seq_t **)realloc(rows, new_cap * sizeof *rows);
    rows_cap = new_cap;
}

/* compute row r (0‑based) and store it in rows[r]                     */
static void compute_row(idx_t r)
{
    seq_t *row = (seq_t *)calloc((size_t)r + 1, sizeof *row);
    for (idx_t k = 0; k <= r; ++k) {
        if (k == 0 || k == 1 || k == r) {
            row[k] = (seq_t)1;
        } else {
            idx_t max_j = (r - k < k - 1) ? r - k : k - 1;   /* min(r‑k, k‑1) */
            seq_t sum = (seq_t)0;
            for (idx_t j = 1; j <= max_j; ++j) {
                seq_t a = rows[r - k][j];
                seq_t b = rows[k - 2][k - j - 1];
                sum = sum + (a * b);
            }
            row[k] = sum;
        }
    }
    rows[r] = row;
}

/* ensure that rows[0 … up_to] are computed                           */
static void ensure_rows_up_to(idx_t up_to)
{
    if (rows_used > up_to) return;
    ensure_rows_cap((size_t)up_to + 1);
    for (idx_t r = (idx_t)rows_used; r <= up_to; ++r) {
        compute_row(r);
    }
    rows_used = (size_t)up_to + 1;
}

/* public sequence function                                              */
seq_t a(idx_t n)
{
    /* find row r such that pref(r‑1) ≤ n < pref(r)                        */
    idx_t low  = 0;
    idx_t high = 1;
    while (pref(high) <= n) high <<= 1;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (pref(mid) <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t row = low;                               /* minimal r with pref(r) > n */
    idx_t prev = (row == 0) ? 0 : pref(row - 1);
    idx_t k = n - prev;                            /* column index in the row */

    ensure_rows_up_to(row);
    return rows[row][k];
}
