/*
 * A126331  Triangle T(n,k), 0 <= k <= n, read by rows defined by: T(0,0)=1,
 *          T(n,k)=0 if k < 0 or if k > n, T(n,0) = 4*T(n-1,0) + T(n-1,1),
 *          T(n,k) = T(n-1,k-1) + 5*T(n-1,k) + T(n-1,k+1) for k >= 1.
 * https://oeis.org/A126331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 28, u32 n <= 91, u64 n <= 325,
 *            u128 n <= 1176, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle T(n,k) defined by:
 *   T(0,0) = 1,
 *   T(n,0) = 4*T(n-1,0) + T(n-1,1)          (n ≥ 1)
 *   T(n,k) = T(n-1,k-1) + 5*T(n-1,k) + T(n-1,k+1)   (k ≥ 1)
 *
 * The sequence a(n) is the triangle read by rows: a(0)=T(0,0),
 * a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), …
 *
 * The function below returns a(n) for any n ≥ 0, using memoisation
 * to keep all previously computed rows in memory.
 * It works for arbitrary seq_t and idx_t types, provided seq_t
 * can hold the required values. The implementation uses only
 * + - * / & | ^ << >> and the integer types.
 */

#include <stdlib.h>   /* malloc, realloc, calloc, abort */
#include <stdint.h>   /* uint64_t, etc */

/* internal storage for the triangle rows.
 * rows[i] points to an array of length i+1 holding T(i,k). */
static seq_t **rows = NULL;
static size_t  rows_cap = 0;   /* number of rows currently allocated */

/* find the largest r such that r*(r+1)/2 <= n.
 * Works for all non‑negative n. */
static size_t find_row(idx_t n)
{
        size_t low = 0;
        size_t high = 1;

        /* grow high until it is enough */
        while ((idx_t)high * (high + 1u) / 2 <= n)
                high <<= 1U;

        /* binary search */
        while (low < high) {
                size_t mid = (low + high + 1u) / 2U;
                if ((idx_t)mid * (mid + 1u) / 2 <= n)
                        low = mid;
                else
                        high = mid - 1U;
        }
        return low;
}

/* ensure that rows 0 … max_r are computed */
static void ensure_rows(size_t max_r)
{
        if (max_r < rows_cap)
                return;   /* already have enough */

        /* grow the rows array */
        size_t new_cap = max_r + 1U;
        rows = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
        if (rows == NULL)
                abort();  /* allocation failed */

        /* compute missing rows */
        for (size_t r = rows_cap; r <= max_r; ++r) {
                seq_t *row = (seq_t*)calloc(r + 1U, sizeof(seq_t));
                if (row == NULL)
                        abort();

                if (r == 0U) {                         /* T(0,0) = 1 */
                        row[0] = 1;
                } else {
                        /* first element: T(r,0) = 4*T(r-1,0) + T(r-1,1) */
                        seq_t a0 = rows[r - 1][0];
                        seq_t b0 = (r >= 2U) ? rows[r - 1][1] : 0;
                        row[0] = 4 * a0 + b0;

                        /* middle elements */
                        for (size_t k = 1U; k < r; ++k) {
                                seq_t left   = rows[r - 1][k - 1];
                                seq_t center = rows[r - 1][k];
                                seq_t right  = (k + 1U <= r - 1U) ? rows[r - 1][k + 1] : 0;
                                row[k] = left + 5 * center + right;
                        }

                        /* last element: T(r,r) = T(r-1,r-1) */
                        row[r] = rows[r - 1][r - 1];
                }
                rows[r] = row;
        }
        rows_cap = new_cap;
}

/* public entry point: return the nth term of the sequence */
seq_t a(idx_t n)
{
        size_t r = find_row(n);                     /* row number */
        size_t k = (size_t)(n - (size_t)r * (r + 1u) / 2);
        ensure_rows(r);                              /* make sure row exists */
        return rows[r][k];
}
