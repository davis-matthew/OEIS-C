/*
 * A137297  Triangle read by rows: coefficients of a Hermite-like set of
 *          recursive polynomials that appear by integration to be orthogonal
 *          using the substitution on the Hermite recursion of n->f(n) where
 *          f(n)=A000045[n] is the Fibonacci sequence.
 * https://oeis.org/A137297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 29, i16 n <= 57, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A137297: Triangle read by rows; coefficients of polynomials defined by
   p(x,0)=1; p(x,1)=x; p(x,n)=x*p(x,n-1)-F(n)*p(x,n-2), where F(n) is
   Fibonacci number A000045[n] (F(0)=0,F(1)=1).  The linear index n starts
   at 1 (a(1)=1). */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/fib.h"

static inline idx_t offset_row(idx_t r)
{
    return r * (r + 1) / 2 + 1;
}

static inline idx_t find_row(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 < n)
        ++r;
    return r;
}

static seq_t **rows = NULL;
static size_t rows_len = 0;
static size_t rows_cap = 0;

static void grow_rows(size_t new_cap)
{
    if (rows_cap >= new_cap)
        return;
    size_t cap = rows_cap ? rows_cap << 1 : 4;
    if (cap < new_cap)
        cap = new_cap;
    seq_t **tmp = (seq_t **)realloc(rows, cap * sizeof(seq_t *));
    rows = tmp;
    rows_cap = cap;
}

static void ensure_rows_to(idx_t r)
{
    while (rows_len <= r)
    {
        idx_t cur = rows_len;
        seq_t *row = (seq_t *)malloc((size_t)(cur + 1) * sizeof(seq_t));
        if (!row)
            abort(); /* memory exhausted */

        if (cur == 0)
        {
            row[0] = 1;
        }
        else if (cur == 1)
        {
            row[0] = 0;
            row[1] = 1;
        }
        else
        {
            seq_t *prev1 = rows[cur - 1];
            seq_t *prev2 = rows[cur - 2];
            seq_t a_n = fib(cur);
            for (idx_t k = 0; k <= cur; ++k)
            {
                seq_t left  = (k > 0) ? prev1[k - 1] : 0;
                seq_t right = (k <= cur - 2) ? prev2[k] : 0;
                row[k] = left - a_n * right;
            }
        }
        grow_rows(rows_len + 1);
        rows[rows_len++] = row;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* out of offset */
    idx_t r = find_row(n);
    ensure_rows_to(r);
    idx_t start = offset_row(r);
    idx_t col   = n - start; /* zero‑based column */
    return rows[r][(size_t)col];
}
