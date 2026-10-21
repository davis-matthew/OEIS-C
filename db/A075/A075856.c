/*
 * A075856  Triangle formed from coefficients of the polynomials p(1)=x,
 *          p(n+1) = (n + x*(n+1))*p(n) + x*x*(d/dx)p(n).
 * https://oeis.org/A075856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   estimate_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A075856:  T(n,k) = (n-1)·T(n-1,k) + (n+k-1)·T(n-1,k-1)
 *  with T(1,1) = 1.
 *  Sequence a(n) is the triangle read by rows:
 *  a(1)=1; a(2)=1; a(3)=3; a(4)=2; … 
 *  Row r satisfies r(r+1)/2 ≥ n > (r−1)r/2,
 *  and column k = n – (r−1)r/2.  Then a(n)=T(r,k).
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/estimate_row.h"

/* static storage for the computed triangle */
static seq_t **rows   = NULL;   /* rows[1..maxRow] each points to an array of length r+1 */
static idx_t   maxRow = 0;      /* largest row that has been computed */
static size_t  rowsCap = 0;     /* capacity of the rows array */

/* Allocate enough space to hold rows 1..r, computing any new rows. */
static void ensure_rows_up_to(idx_t r)
{
    if (r <= maxRow)
        return;

    /* Grow the rows array itself if necessary. */
    if (rowsCap < r + 1) {
        size_t newCap = rowsCap ? rowsCap * 2 : 16;
        while (newCap < r + 1)
            newCap <<= 1;
        /* C++ requires a cast from void* for realloc. */
        seq_t **tmp = (seq_t**)realloc(rows, newCap * sizeof(seq_t*));
        if (!tmp) abort();   /* out of memory */
        rows = tmp;
        rowsCap = newCap;
    }

    /* Compute the new rows. */
    for (idx_t cur = maxRow + 1; cur <= r; ++cur) {
        /* Allocate space for the cur‑th row: indices 1..cur . */
        seq_t *curRow = (seq_t*)malloc((cur + 1) * sizeof(seq_t));
        if (!curRow) abort();
        rows[cur] = curRow;

        /* Special‑case row 1. */
        if (cur == 1) {
            curRow[1] = (seq_t)1;
            continue;
        }

        seq_t *prev = rows[cur - 1];   /* previous row, indices 1..cur-1 */

        for (idx_t k = 1; k <= cur; ++k) {
            seq_t val;
            if (k == 1) {
                /* T(cur,1) = (cur-1)·T(cur-1,1) */
                val = (seq_t)(cur - 1u) * prev[1];
            } else if (k == cur) {
                /* T(cur,cur) = (cur+cur-1)·T(cur-1,cur-1) */
                val = (seq_t)(cur + cur - 1u) * prev[cur - 1];
            } else {
                /* T(cur,k) = (cur-1)·T(cur-1,k) + (cur+k-1)·T(cur-1,k-1) */
                seq_t part1 = (seq_t)(cur - 1u) * prev[k];
                seq_t part2 = (seq_t)(cur + k - 1u) * prev[k - 1];
                val = part1 + part2;
            }
            curRow[k] = val;
        }
    }
    maxRow = r;
}

/* Public function required by the judge. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1 – behaviour not used */

    idx_t r = estimate_row(n);                /* row containing the term */
    idx_t prefix = (r - 1u) * r / 2u;          /* number of terms in previous rows */
    idx_t k = n - prefix;                      /* column index, 1‑based */

    ensure_rows_up_to(r);
    return rows[r][k];
}
