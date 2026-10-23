/*
 * A102587  T(n, k) = (-1)^n*2*[x^k] ChebyshevT(n, (1 - x)/2) with T(0,0) = 1,
 *          for 0 <= k <= n, triangle read by rows.
 * https://oeis.org/A102587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 71, i16 n <= 86, i32 n <= 86, i64 n <= 86, i128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   T(0,0)=1
 *   T(1,0)=-1, T(1,1)=1
 *   T(2,0)=-1, T(2,1)=-2, T(2,2)=1
 *   for n>=3: T(n,k)=T(n-1,k-1)-T(n-1,k)-T(n-2,k)
 *   a(n) is the flattened triangle read by rows.
 * The function memoises the triangle on successive calls. */

#include <stdlib.h>

static seq_t *a_buf   = NULL;  /* flattened triangle          */
static size_t a_cap   = 0;      /* capacity of a_buf           */
static size_t a_len   = 0;      /* number of elements stored   */

static seq_t *prev2   = NULL;  /* T(r-2, *)                  */
static size_t len2    = 0;      /* length of prev2             */
static seq_t *prev1   = NULL;  /* T(r-1, *)                  */
static size_t len1    = 0;      /* length of prev1             */
static idx_t   next_r = 0;      /* index of next row to compute */

seq_t a(idx_t n)
{
    /* initialisation on first call */
    if (!a_buf) {
        a_cap = 64;
        a_buf = (seq_t *)malloc(a_cap * sizeof(seq_t));
        a_len = 0;

        /* row 0 */
        a_buf[a_len++] = (seq_t)1;

        /* row 1 */
        a_buf[a_len++] = (seq_t)-1;
        a_buf[a_len++] = (seq_t)1;

        /* row 2 */
        a_buf[a_len++] = (seq_t)-1;
        a_buf[a_len++] = (seq_t)-2;
        a_buf[a_len++] = (seq_t)1;

        /* allocate base rows for recurrence */
        seq_t *row1 = (seq_t *)malloc(2 * sizeof(seq_t));
        row1[0] = (seq_t)-1;
        row1[1] = (seq_t)1;

        seq_t *row2 = (seq_t *)malloc(3 * sizeof(seq_t));
        row2[0] = (seq_t)-1;
        row2[1] = (seq_t)-2;
        row2[2] = (seq_t)1;

        /* set up pointers for future recurrence */
        prev2 = row1;  len2 = 2;
        prev1 = row2;  len1 = 3;
        next_r = 3;    /* next row index to generate */
    }

    /* grow the triangle up to the requested index */
    while (a_len <= (size_t)n) {
        idx_t r = next_r;                   /* current row index */
        size_t cur_len = (size_t)r + 1;     /* number of elements in this row */

        /* allocate array for the current row */
        seq_t *row_curr = (seq_t *)malloc(cur_len * sizeof(seq_t));
        for (idx_t k = 0; k < (idx_t)cur_len; ++k) {
            seq_t val = 0;
            if (k > 0 && (size_t)(k - 1) < len1)
                val += prev1[(size_t)(k - 1)];
            if ((size_t)k < len1)
                val -= prev1[(size_t)k];
            if ((size_t)k < len2)
                val -= prev2[(size_t)k];
            row_curr[(size_t)k] = val;
        }

        /* ensure enough space in the flattened buffer */
        if (a_len + cur_len > a_cap) {
            size_t new_cap = a_cap * 2;
            if (new_cap < a_len + cur_len) new_cap = a_len + cur_len;
            seq_t *new_buf = (seq_t *)malloc(new_cap * sizeof(seq_t));
            for (size_t i = 0; i < a_len; ++i)
                new_buf[i] = a_buf[i];
            free(a_buf);
            a_buf = new_buf;
            a_cap = new_cap;
        }

        /* append the newly computed row */
        for (size_t i = 0; i < cur_len; ++i)
            a_buf[a_len++] = row_curr[i];

        /* shift rows for next iteration */
        free(prev2);
        prev2 = prev1;  len2 = len1;
        prev1 = row_curr;  len1 = cur_len;

        ++next_r;
    }

    return a_buf[(size_t)n];
}
