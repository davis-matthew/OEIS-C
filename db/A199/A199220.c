/*
 * A199220  Triangle read by rows: T(n,k) = (n-1-k)*abs(s(n,n+1-k)), where
 *          s(n,k) are the signed Stirling numbers of the first kind and 1 <=
 *          k <= n.
 * https://oeis.org/A199220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = (n-1-k)*abs(s(n,n+1-k)) read row‑wise, where s(n,k) are
 *            the signed Stirling numbers of the first kind
 * Range:     i8 n <= 17, i16 n <= 40, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Private cache of signed Stirling numbers of the first kind. */
static seq_t **stirling_cache = NULL; /* array of row pointers */
static idx_t  max_cached   = 0;       /* largest row currently cached */

/* Extend the Stirling cache up to row 'limit' (inclusive). */
static void build_stirling(idx_t limit)
{
    if (limit <= max_cached)
        return;                     /* already built */

    /* Resize the array of row pointers. */
    seq_t **new_cache = (seq_t**)realloc(stirling_cache,
                                        (size_t)(limit + 1) * sizeof(seq_t*));
    if (!new_cache)
        abort();
    stirling_cache = new_cache;

    /* Initialise row 0 if starting from scratch. */
    if (max_cached == 0 && limit >= 0) {
        stirling_cache[0] = (seq_t*)malloc(sizeof(seq_t));
        if (!stirling_cache[0])
            abort();
        stirling_cache[0][0] = 1;    /* s(0,0) = 1 */
    }

    /* Compute rows from (max_cached+1) up to 'limit'. */
    for (idx_t i = max_cached + 1; i <= limit; ++i) {
        /* Allocate row i (size i+1, indices 0..i). */
        stirling_cache[i] = (seq_t*)calloc((size_t)(i + 1), sizeof(seq_t));
        if (!stirling_cache[i])
            abort();

        for (idx_t k = 1; k <= i; ++k) {
            seq_t prev_left = stirling_cache[i - 1][k - 1];
            seq_t prev_mid = (k <= i - 1) ? stirling_cache[i - 1][k] : 0;
            /* s(i,k) = s(i-1,k-1) - (i-1)*s(i-1,k) */
            seq_t term = prev_left - (seq_t)(i - 1) * prev_mid;
            stirling_cache[i][k] = term;
        }
        /* s(i,0) is already zero due to calloc. */
    }

    max_cached = limit;
}

/* Convert a flattened index (1‑based) to row and column (both 1‑based). */
static void index_to_row_col(idx_t idx, idx_t *row, idx_t *col)
{
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < idx) ++r;   /* find row */
    *row = r;
    idx_t prev_tri = (r - 1) * r / 2;
    *col = idx - prev_tri;                /* 1‑based column inside the row */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                         /* out of range */

    idx_t row, col;
    index_to_row_col(n, &row, &col);

    /* Ensure the required Stirling numbers are cached. */
    build_stirling(row);

    idx_t j = row + 1 - col;              /* index of the Stirling number s(n,n+1-k) */
    seq_t s_val = stirling_cache[row][j];

    /* Compute the multiplier (n-1-k) safely as a signed difference. */
    seq_t multiplier = (seq_t)(row - 1) - (seq_t)col;

    seq_t abs_s = s_val >= 0 ? s_val : -s_val;
    return multiplier * abs_s;
}
