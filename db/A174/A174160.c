/*
 * A174160  Triangle read by rows: T(n,m) = 2*Eulerian(n, m-1) -
 *          (binomial(n-1, m-1) * binomial(n, m-1) / m)^2.
 * https://oeis.org/A174160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the triangle read by rows where T(n,m) = 2*Eulerian(n,
 *            m-1) - (binomial(n-1, m-1) * binomial(n, m-1) / m)^2. Sequence
 *            starts at a(1) = 1, offset 1
 * Range:     i8 n <= 12, i16 n <= 31, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t **eulerian_cache = NULL;  /* eulerian_cache[n][k] = <n,k> for 0<=k<n */
static idx_t   eulerian_rows   = 0;    /* highest n that has been cached */

/* Ensure Eulerian numbers are computed up to 'upto' (inclusive). */
static void ensure_eulerian(idx_t upto)
{
    if (upto <= eulerian_rows)
        return;

    /* grow cache array */
    seq_t **tmp = (seq_t **)realloc(eulerian_cache, (upto + 1) * sizeof(seq_t *));
    eulerian_cache = tmp;

    /* compute missing rows */
    for (idx_t n = eulerian_rows + 1; n <= upto; ++n)
    {
        /* allocate row of size n (indices 0 .. n-1) */
        seq_t *row = (seq_t *)calloc((size_t)n, sizeof(seq_t));
        if (row == NULL)
            return; /* out of memory */

        if (n == 0 || n == 1) {
            /* <0,0> = <1,0> = 1 */
            row[0] = 1;
        } else {
            for (idx_t k = 0; k < n; ++k)
            {
                seq_t val = 0;
                if (k > 0) {
                    val += (seq_t)(n - k) * eulerian_cache[n - 1][k - 1];
                }
                if (k < n - 1) {   /* term with A(n-1,k) */
                    val += (seq_t)(k + 1) * eulerian_cache[n - 1][k];
                }
                row[k] = val;
            }
        }
        eulerian_cache[n] = row;
    }

    eulerian_rows = upto;
}

/* Return row index r (1‑based) such that the n-th element lies in row r. */
static idx_t row_of_n(idx_t n)
{
    idx_t r = 0;
    while (r * (r + 1) / 2 < n)
        ++r;
    return r;
}

/* Return the n-th term of A174160. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* sequence defined for n >= 1 only */

    idx_t row = row_of_n(n);          /* 1‑based row number */
    idx_t prev_sum = (row - 1) * row / 2;  /* number of terms before this row */
    idx_t m = n - prev_sum;               /* column index (1‑based) */

    /* compute Eulerian(row, m-1) */
    ensure_eulerian(row);
    seq_t euler = eulerian_cache[row][m - 1];

    /* compute the binomial part */
    seq_t bin1 = binomial(row - 1, m - 1);
    seq_t bin2 = binomial(row,     m - 1);
    seq_t prod  = bin1 * bin2;
    seq_t part  = prod / (seq_t)m;          /* guaranteed integer division */
    seq_t part_sq = part * part;

    seq_t result = 2 * euler - part_sq;
    return result;
}
