/*
 * A073149  Triangle of numbers arising in recursive computation of A002212.
 * https://oeis.org/A073149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for triangle T(n,k) of A073149 and flattened sequence a(n):
   T(0,0)      = 1
   T(1,0)      = 1
   T(n+1,0)    = T(n,0) + T(n,n)                 (n > 0)
   T(n,k)      = T(n,k-1) + T(k,0) * T(n-k,0)    (k > 0)
   a(0) = T(0,0), a(1) = T(1,0), a(2)=T(1,1), a(3)=T(2,0), … */

#include <stdlib.h>

static seq_t **rows_cache = NULL;   /* rows_cache[i] points to array of size i+1 */
static size_t rows_cache_size = 0;  /* allocated size of rows_cache pointer array */
static size_t rows_computed = 0;    /* next row index to compute */

/* Ensure that rows 0 … idx are computed and cached. */
static void ensure_row_cache(size_t idx)
{
    if (rows_computed > idx) return;   /* already done */

    /* Grow the array of row pointers if necessary. */
    if (idx >= rows_cache_size) {
        size_t new_size = rows_cache_size ? rows_cache_size * 2 : 4;
        while (new_size <= idx) new_size <<= 1;
        /* cast to seq_t** so this also works in C++ */
        seq_t **tmp = (seq_t **)realloc(rows_cache, new_size * sizeof(seq_t *));
        if (!tmp) abort();
        rows_cache = tmp;
        rows_cache_size = new_size;
    }

    /* Compute rows incrementally until idx. */
    for (; rows_computed <= idx; ++rows_computed) {
        size_t n = rows_computed;
        /* cast to seq_t* for C++ compatibility */
        seq_t *row = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!row) abort();
        rows_cache[n] = row;

        if (n == 0) {               /* T(0,0) */
            row[0] = 1;
        } else if (n == 1) {        /* T(1,0) and T(1,1) */
            row[0] = 1;
            /* T(1,1) = T(1,0) + T(1,0)*T(0,0) */
            row[1] = row[0] + row[0] * rows_cache[0][0];
        } else {                    /* n >= 2 */
            row[0] = rows_cache[n-1][0] + rows_cache[n-1][n-1];
            for (size_t k = 1; k <= n; ++k) {
                row[k] = row[k-1] + rows_cache[k][0] * rows_cache[n-k][0];
            }
        }
    }
}

/* Return the n-th term of the flattened triangle. */
seq_t a(idx_t n)
{
    /* Determine the row r that contains index n.
       The first index of row r is r*(r+1)/2. */
    size_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= (size_t)n) r++;

    size_t start = r * (r + 1) / 2;
    size_t col   = (size_t)n - start;

    ensure_row_cache(r);

    return rows_cache[r][col];
}
