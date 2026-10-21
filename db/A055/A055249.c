/*
 * A055249  Triangle of partial row sums (prs) of triangle A055248 (prs of
 *          Pascal's triangle A007318).
 * https://oeis.org/A055249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 104, u32 n <= 434, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   row_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A055249 – triangle of partial row sums of the reversed
 * Pascal triangle (prs of Pascal's triangle).
 *
 * For index n ≥ 0 we first determine the row r and column c of the
 * lower‑triangular array:
 *     n = r*(r+1)/2 + c   ,   0 ≤ c ≤ r
 *
 * For a given row r we precompute
 *
 *   binom[r][k] = C(r,k)
 *   tail[k]     = Σ_{t=k..r} C(r,t)      (partial sums from the right)
 *
 * The element a(r,c) is the sum of the tail entries from c to r:
 *
 *   a(r,c) = Σ_{k=c..r} tail[k]
 *
 * 
 * The function a(idx_t n) below implements this formula.  It uses
 * static storage for rows that have already been computed so repeated
 * calls are O(r) with negligible overhead.  All arithmetic is done
 * in the type seq_t (which may be an unsigned integer type or an
 * arbitrary‑precision integer class).
 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, realloc */
#include "../utils/row_index.h"

typedef struct {
    seq_t *binom;  /* C(r,k), 0 ≤ k ≤ r  */
    seq_t *tail;   /* Σ_{t=k..r} C(r,t)   */
} RowInfo;

/* Static storage for rows that have already been computed. */
static RowInfo *row_array = NULL;
static size_t rows_used = 0;
static size_t rows_cap = 0;

static void
ensure_row(idx_t r)
{
    if (r >= rows_cap) {
        size_t new_cap = (rows_cap == 0 ? 8 : rows_cap * 2);
        if (new_cap < r + 1)
            new_cap = r + 1;
        RowInfo *tmp = (RowInfo *)realloc(row_array, new_cap * sizeof(RowInfo));
        if (!tmp) abort();                /* memory exhausted */
        for (size_t i = rows_cap; i < new_cap; ++i) {
            tmp[i].binom = NULL;
            tmp[i].tail  = NULL;
        }
        row_array = tmp;
        rows_cap = new_cap;
    }
    if (row_array[r].binom != NULL)
        return;   /* already computed */

    /* Allocate space for binomial coefficients and tails */
    RowInfo *ri = &row_array[r];
    ri->binom = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    ri->tail  = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    if (!ri->binom || !ri->tail) abort();

    /* Compute binomial coefficients: C(r,0)=1, C(r,k)=C(r,k-1)*(r-k+1)/k */
    ri->binom[0] = 1;
    for (idx_t k = 1; k <= r; ++k) {
        seq_t num = ri->binom[k - 1] * (seq_t)(r - k + 1);
        ri->binom[k] = num / (seq_t)k;   /* exact division */
    }

    /* Compute tail sums from right to left: tail[r] = C(r,r) */
    ri->tail[r] = ri->binom[r];
    for (idx_t k = r; k > 0; --k)
        ri->tail[k - 1] = ri->tail[k] + ri->binom[k - 1];
}

seq_t a(idx_t n)
{
    /* Determine row r and column c of the flattened index n. */
    idx_t r = row_index(n);
    idx_t c = n - r * (r + 1) / 2;

    /* Ensure the row data is available. */
    ensure_row(r);
    RowInfo *ri = &row_array[r];

    /* Sum tail values from column c to r. */
    seq_t result = 0;
    for (idx_t k = c; k <= r; ++k)
        result += ri->tail[k];

    return result;
}
