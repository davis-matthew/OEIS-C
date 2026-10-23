/*
 * A171414  Triangle read by rows (n >= 1): T(n,k) = [x^k] p(x,n), where
 *          p(x,n) = ((x^n - 1)/(x - 1))^floor(n/2) if n is odd, and p(x,n) =
 *          ((x^n - 1)/(x - 1))*p(x,n-1) otherwise.
 * https://oeis.org/A171414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   add_to_span
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A171414: Triangle read by rows.  
 *   T(n,k) = coefficient of x^k in p(x,n), where
 *   p(x,n) = ((x^n - 1)/(x - 1))^floor(n/2) if n is odd,
 *   p(x,n) = ((x^n - 1)/(x - 1))*p(x,n-1) otherwise.
 *   The sequence a(n) is the flattened triangle read by rows.
 *   a(n) = T(row, pos) where row is the smallest integer with
 *   sum_{i=1}^{row} (deg(p(x,i))+1) >= n, and pos = n - sum_{i=1}^{row-1} (deg(p(x,i))+1).
 */

#include <stdlib.h>
#include "../utils/add_to_span.h"

/* Compute the required triangle rows up to index n and return a(n). */
seq_t a(idx_t n) {
    /* static state to keep rows across calls */
    static seq_t **rows = NULL;      /* array of pointers to coeff arrays, 1‑based */
    static size_t *row_len = NULL;   /* length of each row, 1‑based */
    static size_t *cum = NULL;       /* cumulative number of terms, 1‑based */
    static size_t cap = 0;           /* capacity of the static arrays */
    static size_t rows_cnt = 0;      /* number of rows computed so far */

    /* -- helper to extend static arrays if needed ----------------------- */
    if (rows_cnt == 0) {
        /* initial allocation */
        cap = 16;
        rows = (seq_t**)malloc(cap * sizeof(seq_t*));
        row_len = (size_t*)malloc(cap * sizeof(size_t));
        cum = (size_t*)malloc(cap * sizeof(size_t));
        cum[0] = 0;
    } else if (rows_cnt + 1 >= cap) {
        /* double capacity */
        size_t new_cap = cap * 2;
        rows = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
        row_len = (size_t*)realloc(row_len, new_cap * sizeof(size_t));
        cum = (size_t*)realloc(cum, new_cap * sizeof(size_t));
        cap = new_cap;
    }

    /* ------------------------------------------------------------------ */
    /* Precompute rows until cumulative terms cover the requested index n */
    while (n > cum[rows_cnt]) {
        size_t r = rows_cnt + 1;          /* new row index (1‑based) */
        size_t len_r;                     /* length of the new row */

        if (r == 1) {
            /* base case: p(x,1) = 1 */
            rows[1] = (seq_t*)malloc(sizeof(seq_t));
            rows[1][0] = (seq_t)1;
            len_r = 1;
        } else if (r % 2 == 0) {
            /* even row: p(x,r) = (1+x+...+x^{r-1}) * p(x,r-1) */
            size_t prev_len = row_len[r - 1];
            size_t base_len = r;           /* coefficients of 1+x+...+x^{r-1} are all 1 */
            len_r = prev_len + base_len - 1;
            seq_t *new_arr = (seq_t*)calloc(len_r, sizeof(seq_t));
            seq_t *prev_arr = rows[r - 1];
            for (size_t i = 0; i < prev_len; ++i) {
                seq_t val = prev_arr[i];
                /* add val to span of length base_len starting at position i */
                add_to_span(new_arr, i, base_len, val);
            }
            rows[r] = new_arr;
        } else {
            /* odd row: p(x,r) = (1+x+...+x^{r-1})^(r/2) */
            size_t m = r / 2;
            size_t base_len = r;           /* all coefficients are 1 */
            /* start with polynomial 1 */
            seq_t *cur = (seq_t*)calloc(1, sizeof(seq_t));
            cur[0] = (seq_t)1;
            size_t cur_len = 1;
            for (size_t step = 0; step < m; ++step) {
                size_t new_len = cur_len + base_len - 1;
                seq_t *next_arr = (seq_t*)calloc(new_len, sizeof(seq_t));
                for (size_t i = 0; i < cur_len; ++i) {
                    seq_t val = cur[i];
                    add_to_span(next_arr, i, base_len, val);
                }
                free(cur);
                cur = next_arr;
                cur_len = new_len;
            }
            rows[r] = cur;
            len_r = cur_len;
        }

        row_len[r] = len_r;
        cum[r] = cum[r - 1] + len_r;
        ++rows_cnt;
    }

    /* ------------------------------------------------------------------ */
    /* Find the row containing the n-th term (binary search on cum array) */
    size_t lo = 1, hi = rows_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (cum[mid] >= (size_t)n) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    size_t target_row = lo;
    size_t offset = (size_t)n - cum[target_row - 1] - 1;  /* 0‑based index in row */

    return rows[target_row][offset];
}
