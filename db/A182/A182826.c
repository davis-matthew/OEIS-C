/*
 * A182826  Coefficient array for orthogonal polynomials
 *          p(n,x)=(x-(2n-1))*p(n-1,x)-(2n-2)^2*p(n-2,x), p(0,x)=1,p(1,x)=x-1.
 * https://oeis.org/A182826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) is given by flattening the coefficient array of the
 *            orthogonal polynomials p(n,x) defined by p(0,x)=1, p(1,x)=x-1,
 *            p(n,x)=(x-(2n-1))*p(n-1,x)-(2n-2)^2*p(n-2,x)
 * Range:     i8 n <= 10, i16 n <= 27, i32 n <= 58, i64 n <= 58, i128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* forward declaration of the static helper */
static void ensure_row(idx_t r);
static idx_t find_row(idx_t n);

/* Global state for memoised rows of coefficients */
static seq_t **rows       = NULL;  /* pointer to each coefficient row */
static idx_t  max_cached  = 0;     /* highest row index computed so far */
static size_t capacity    = 0;     /* size of the rows array */

/* Ensure that rows[0..r] have been computed. */
static void ensure_row(idx_t r)
{
    /* Lazy initialisation of row 0 */
    if (rows == NULL) {
        capacity = 1;
        rows = (seq_t**)malloc(capacity * sizeof(seq_t*));
        seq_t *row0 = (seq_t*)malloc(sizeof(seq_t));
        row0[0] = 1;
        rows[0] = row0;
        max_cached = 0;
    }

    while (max_cached < r) {
        idx_t cur = max_cached + 1;
        seq_t *row;

        if (cur == 0) {                        /* p(0,x)=1 */
            row = (seq_t*)malloc(sizeof(seq_t));
            row[0] = 1;
        } else if (cur == 1) {                 /* p(1,x)=x-1 */
            row = (seq_t*)malloc(2 * sizeof(seq_t));
            row[0] = -1;
            row[1] = 1;
        } else {                               /* general recurrence */
            seq_t *q = rows[cur-1];            /* p(cur-1,x) */
            seq_t *p = rows[cur-2];            /* p(cur-2,x) */
            row = (seq_t*)malloc((cur + 1) * sizeof(seq_t));

            seq_t step1  = (seq_t)(2 * cur - 1);   /* (2n-1)   */
            seq_t step2  = (seq_t)(2 * cur - 2);   /* (2n-2)   */
            seq_t step2sq = step2 * step2;        /* (2n-2)^2 */

            for (idx_t i = 0; i <= cur; ++i) {
                seq_t val = (i > 0 ? q[i-1] : 0); /* x·q term          */
                if (i <= cur - 1)                /* -(2n-1)·q[i]      */
                    val -= step1 * q[i];
                if (i <= cur - 2)                /* -(2n-2)^2·p[i]    */
                    val -= step2sq * p[i];
                row[i] = val;
            }
        }

        /* Grow the rows array if required. */
        if (cur >= capacity) {
            size_t new_cap = capacity ? capacity * 2 : 4;
            while (new_cap <= cur)
                new_cap <<= 1;
            rows = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
            capacity = new_cap;
        }

        rows[cur] = row;
        ++max_cached;
    }
}

/* Find r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
static idx_t find_row(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;

    /* Expand high until it is large enough. */
    while (((seq_t)high * (high + 1) / 2) <= n)
        high <<= 1;

    /* Binary search on [low,high]. */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (((seq_t)mid * (mid + 1) / 2) <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Main accessor for the OEIS sequence A182826. */
seq_t a(idx_t n)
{
    idx_t r      = find_row(n);
    idx_t offset = (idx_t)(((seq_t)r * (r + 1)) / 2);
    idx_t c      = n - offset;               /* column within the row    */
    ensure_row(r);
    return rows[r][c];
}
