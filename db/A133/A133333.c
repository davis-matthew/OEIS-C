/*
 * A133333  Irregular triangle read by rows: coefficients of polynomial
 *          (x^(n-1) - (1+x+...+x^(n-2))) ^ (n+1).
 * https://oeis.org/A133333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) is the coefficient of x^{k} in (x^(r-1) -
 *            (1+x+...+x^(r-2)))^(r+1) where r is the row of the irregular
 *            triangle containing the n‑th term
 * Range:     i8 n <= 35, i16 n <= 73, i32 n <= 73, i64 n <= 73, i128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t **row_cache = NULL; /* row_cache[r-1] holds coefficients of row r */
static idx_t cached_rows = 0;    /* number of rows currently allocated */

static void compute_row(idx_t r)
{
    idx_t m = r * r;                 /* number of coefficients in row r */
    /* allocate storage for the row in the cache */
    seq_t *row = (seq_t *)malloc((size_t)m * sizeof(seq_t));
    if (!row) exit(EXIT_FAILURE);

    /* arrays for polynomial multiplication */
    seq_t *cur = (seq_t *)malloc((size_t)m * sizeof(seq_t));
    seq_t *next = (seq_t *)malloc((size_t)m * sizeof(seq_t));
    if (!cur || !next) exit(EXIT_FAILURE);

    /* base polynomial B(x) = x^(r-1) - (1+x+...+x^(r-2)) */
    seq_t *base = (seq_t *)malloc((size_t)r * sizeof(seq_t));
    if (!base) exit(EXIT_FAILURE);
    for (idx_t i = 0; i < r - 1; ++i) base[i] = -1;
    base[r - 1] = 1;

    /* start with polynomial `1` */
    for (idx_t i = 0; i < m; ++i) cur[i] = 0;
    cur[0] = 1;
    idx_t cur_len = 1;

    /* multiply base polynomial r+1 times */
    for (idx_t it = 0; it < r + 1; ++it) {
        idx_t new_len = cur_len + r - 1;
        /* zero next array up to new_len */
        for (idx_t i = 0; i < new_len; ++i) next[i] = 0;
        /* convolution */
        for (idx_t i = 0; i < cur_len; ++i) {
            seq_t ci = cur[i];
            if (ci == 0) continue;
            for (idx_t j = 0; j < r; ++j) {
                next[i + j] += ci * base[j];
            }
        }
        /* swap cur and next */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
        cur_len = new_len;
    }

    /* copy final coefficients into the row cache */
    for (idx_t i = 0; i < m; ++i) row[i] = cur[i];
    row_cache[r - 1] = row;

    /* free temporary arrays */
    free(cur);
    free(next);
    free(base);
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined below the offset */

    /* find the row `r` that contains position n */
    idx_t r = 1;
    idx_t sum = 1;          /* sum of k^2 for k=1..r */
    while (sum < n) {
        ++r;
        sum += r * r;
    }
    /* index within the row (0‑based) */
    idx_t p = n - (sum - r * r) - 1;   /* n - sum_{k=1}^{r-1}k^2 - 1 */

    /* make sure the cache can hold row r */
    if (r > cached_rows) {
        idx_t new_size = r;
        seq_t **tmp = (seq_t **)malloc((size_t)new_size * sizeof(seq_t *));
        if (!tmp) exit(EXIT_FAILURE);
        for (idx_t i = cached_rows; i < new_size; ++i) tmp[i] = NULL;
        row_cache = tmp;
        cached_rows = new_size;
    }

    /* compute the row on demand */
    if (!row_cache[r - 1]) compute_row(r);

    return row_cache[r - 1][p];
}
