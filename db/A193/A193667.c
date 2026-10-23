/*
 * A193667  Triangular array: the fission of (p(n,x)) by (q(n,x)), where
 *          p(n,x)=(x+1)^n and q(n,x)=sum{F(k+1)*x^(n-k) : 0<=k<=n}, where
 *          F=A000045 (Fibonacci numbers).
 * https://oeis.org/A193667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   R(r,0) = 1
   R(r,k) = R(r-1,k-1) + R(r-1,k)       (0 < k < r)
   R(r,r) = R(r-1,r-1) + F(2r+1)        (F is Fibonacci, F(0)=0,F(1)=1)
   The sequence a(n) enumerates the rows of R row‑by‑row (row 0 has one
   element, row 1 has two, etc.).  All arithmetic is performed in seq_t. */

#include <stdlib.h>

/* ---- static storage for the triangular array ---- */
static seq_t **rows = NULL;          /* rows[r] -> array of r+1 elements   */
static size_t rows_cnt = 0;          /* number of rows currently stored    */
static size_t rows_cap = 0;          /* capacity of the rows[] array       */

/* ---- static storage for Fibonacci numbers ---- */
static seq_t *fib = NULL;            /* Fibonacci numbers F[0..]           */
static size_t fib_len = 0;           /* number of elements currently stored */

static void ensure_rows_cap(size_t needed)
{
    if (rows_cap >= needed) return;
    size_t new_cap = rows_cap ? rows_cap * 2 : 2;
    while (new_cap < needed) new_cap <<= 1;
    rows = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
    if (!rows) abort();
    rows_cap = new_cap;
}

static void ensure_fib(size_t needed)
{
    /* ensure that fib[0..needed] are available (needed is an index) */
    if (fib_len > needed) return;          /* already have them */

    size_t new_len = fib_len ? fib_len * 2 : 2;
    if (new_len <= needed + 1) new_len = needed + 2;  /* need at least needed+1 elements */
    fib = (seq_t*)realloc(fib, new_len * sizeof(seq_t));
    if (!fib) abort();

    if (fib_len == 0) {                    /* initialise only once */
        fib[0] = 0;
        fib[1] = 1;
        fib_len = 2;
    }
    for (size_t i = fib_len; i <= needed; ++i) {
        fib[i] = fib[i-1] + fib[i-2];
    }
    fib_len = needed + 1;
}

static void ensure_row(idx_t r)
{
    /* generate all rows up to and including r */
    while (rows_cnt <= r)
    {
        idx_t len = rows_cnt + 1;          /* number of elements in new row */
        seq_t *row = (seq_t*)malloc(len * sizeof(seq_t));
        if (!row) abort();

        row[0] = 1;                       /* first element is always 1 */

        if (rows_cnt > 0)
        {
            seq_t *prev = rows[rows_cnt - 1];

            /* interior elements (0 < k < r) */
            for (size_t k = 1; k < (size_t)rows_cnt; ++k)
                row[k] = prev[k-1] + prev[k];

            /* make sure we have F(2*rows_cnt+1) available */
            ensure_fib(2 * (size_t)rows_cnt + 1);

            /* last element */
            row[rows_cnt] = prev[rows_cnt - 1] + fib[2 * (size_t)rows_cnt + 1];
        }

        /* store the row */
        ensure_rows_cap(rows_cnt + 1);
        rows[rows_cnt] = row;
        ++rows_cnt;
    }
}

seq_t a(idx_t n)
{
    /* find row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2   */
    idx_t start = 0;
    idx_t r = 0;
    while (r + 1 <= n - start) {
        start += r + 1;    /* start = r(r+1)/2 for the next r */
        ++r;
    }
    idx_t k = n - start;     /* column within row r */

    ensure_row(r);           /* make sure rows[0..r] exist */

    return rows[r][k];
}
