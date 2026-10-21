/*
 * A207605  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A106195; see the Formula section.
 * https://oeis.org/A207605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth element of the triangle T(n,k) defined by
 *            T(1,0)=1, T(2,0)=2, T(2,1)=0, and for n≥3,k≥1
 *            T(n,k)=2*T(n-1,k)+T(n-1,k-1)-T(n-2,k-1). The sequence a(n) is
 *            the triangle flattened row by row (offset 1)
 * Range:     u8 n <= 29, u16 n <= 94, u32 n <= 330, u64 n <= 1188,
 *            u128 n <= 4581, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"

/* ----------  Cached triangle rows ------------------------------------- */
static seq_t **triangle = NULL;   /* triangle[i] points to row i+1  */
static size_t *row_len  = NULL;   /* length of each row             */
static idx_t   max_rows = 0;      /* number of rows already built   */
static idx_t   capacity = 0;      /* capacity of the above arrays  */

/* Ensure the storage arrays can hold at least 'needed' rows. */
static void ensure_capacity(idx_t needed)
{
    if (needed <= capacity) return;
    size_t new_cap = capacity ? capacity * 2 : 16;
    while (new_cap < (size_t)needed) new_cap <<= 1;

    triangle = (seq_t**)realloc(triangle, new_cap * sizeof(seq_t *));
    row_len  = (size_t*)realloc(row_len,  new_cap * sizeof(size_t));

    /* realloc may move memory; simply updates pointers */
    capacity = new_cap;
}

/* Build row number 'n' (1‑based) and append it to the cached triangle. */
static void build_row(idx_t n)
{
    ensure_capacity(max_rows + 1);
    size_t len = (n == 1 || n == 2) ? 1 : (size_t)(n - 1);
    seq_t *row = (seq_t*)malloc(len * sizeof(seq_t));

    if (n == 1) {
        row[0] = 1;
    } else if (n == 2) {
        row[0] = 2;
    } else {
        /* first element is 2^(n-1) */
        row[0] = pow2((idx_t)(n - 1));

        seq_t *prev      = triangle[max_rows - 1];
        size_t len_prev  = row_len[max_rows - 1];
        seq_t *prevprev  = triangle[max_rows - 2];
        size_t len_pprev = row_len[max_rows - 2];

        for (idx_t k = 1; k < (idx_t)len; ++k) {
            seq_t val = 0;
            if ((idx_t)k < (idx_t)len_prev)        val += 2 * prev[k];
            if ((idx_t)(k - 1) < (idx_t)len_prev)  val += prev[k - 1];
            if ((idx_t)(k - 1) < (idx_t)len_pprev) val -= prevprev[k - 1];
            row[(size_t)k] = val;
        }
    }

    triangle[max_rows] = row;
    row_len[max_rows]  = len;
    ++max_rows;
}

/* Return the n‑th term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safe fallback for out‑of‑range */

    /* Find the row that contains the n‑th element. */
    idx_t m   = 1;          /* current row number (1‑based) */
    idx_t cum = 0;          /* cumulative number of elements so far */
    for (;;) {
        size_t len = (m == 1 || m == 2) ? 1 : (size_t)(m - 1);
        if (cum + (idx_t)len >= n) break;
        cum += (idx_t)len;
        ++m;
    }
    idx_t offset = n - cum - 1;   /* 0‑based column index */

    /* Build rows up to 'm' if they are not yet cached. */
    while (max_rows < m) build_row(max_rows + 1);

    return triangle[m - 1][(size_t)offset];
}
