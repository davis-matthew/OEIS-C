/*
 * A202209  Triangle T(n,k), read by rows, given by (2, 1/2, 1/2, 0, 0, 0, 0,
 *          0, 0, 0, ...) DELTA (0, 1/2, -1/2, 0, 0, 0, 0, 0, 0, 0, ...) where
 *          DELTA is the operator defined in A084938.
 * https://oeis.org/A202209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1) - 1) / 2), k = n -
 *            r*(r+1)/2. T(n,k) is defined by T(0,0) = 1, T(n,0) = F_{2n+1}
 *            (Fibonacci; use fib(2*n+1)) T(n,k) = 3*T(n-1,k) - T(n-2,k) +
 *            T(n-2,k-1) (k>0) (Out‑of‑range indices are treated as 0.)
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include "../utils/fib.h"   /* fib(idx_t n) */

static seq_t **rows = NULL;          /* array of pointers to rows */
static idx_t  rows_count = 0;        /* number of rows currently stored */
static idx_t  rows_cap   = 0;        /* allocated capacity */

/* Ensure that rows[0..r] are computed and stored */
static void ensure_row(idx_t r)
{
    /* Grow the rows pointer array if needed */
    if (rows_count == 0) {
        rows_cap = 8;
        rows = (seq_t**)malloc(rows_cap * sizeof(seq_t*));
        if (!rows) return; /* allocation failure; caller will handle nulls */
    }
    while (rows_count <= r) {
        /* grow the pointer array */
        if (rows_count >= rows_cap) {
            rows_cap *= 2;
            seq_t **tmp = (seq_t**)realloc(rows, rows_cap * sizeof(seq_t*));
            if (!tmp) return;
            rows = tmp;
        }

        /* allocate and compute the next row (index = rows_count) */
        idx_t n = rows_count;
        seq_t *row = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        if (!row) return;

        /* T(n,0) = F_{2n+1} */
        row[0] = fib((idx_t)(2 * n + 1));

        /* compute k>=1 entries */
        for (idx_t k = 1; k <= n; ++k) {
            seq_t a = 0, b = 0, c = 0;

            if (n >= 1 && k <= n - 1)   a = rows[n - 1][k];
            if (n >= 2 && k <= n - 2)   b = rows[n - 2][k];
            if (n >= 2 && k - 1 <= n - 2) c = rows[n - 2][k - 1];

            row[k] = 3 * a - b + c;
        }
        rows[n] = row;
        rows_count++;
    }
}

seq_t a(idx_t n)
{
    /* Compute r and k from n */
    long double sqrt_term = sqrt((long double)1.0 + (long double)8.0 * (long double)n);
    idx_t r = (idx_t)((sqrt_term - 1.0L) / 2.0L);
    idx_t start_of_row = r * (r + 1) / 2;
    idx_t k = n - start_of_row;

    ensure_row(r);
    return rows[r][k];
}
