/*
 * A055830  Triangle T read by rows: diagonal differences of triangle A037027.
 * https://oeis.org/A055830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 138, u32 n <= 441, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A055830: a(n) is the flattened triangle T read by rows.
    Recurrence:
        T(0,0) = 1
        T(1,0) = Fib(2) = 1
        T(1,1) = 0
        T(i,0) = Fib(i+1)          for i ≥ 2
        T(i,j) = T(i-1,j-1) + T(i-1,j) + T(i-2,j)   for i ≥ 2, j ≥ 1
        T(i,j) = 0                for j > i or j < 0
    a(n) is the entry T(r,k) where r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 and k = n‑r*(r+1)/2. */

#include <stdlib.h>

static seq_t *fib = NULL;                  /* Fibonacci array (fib[i] = Fib(i)) */
static size_t fib_count = 0;                /* number of computed Fibonacci values */
static size_t fib_cap   = 0;                /* allocated capacity of fib[] */

static void ensure_fib(size_t need)
{
    if (fib_count > need) return;          /* already computed */
    if (fib_cap <= need) {                /* grow capacity */
        size_t new_cap = fib_cap ? fib_cap : 1;
        while (new_cap <= need) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(fib, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        fib   = tmp;
        fib_cap = new_cap;
    }
    if (fib_count == 0) {                 /* initialise first two Fibonacci numbers */
        fib[0] = (seq_t)0;
        fib[1] = (seq_t)1;
        fib_count = 2;
    }
    for (size_t i = fib_count; i <= need; ++i)
        fib[i] = fib[i-1] + fib[i-2];
    fib_count = need + 1;
}

static seq_t **triangle = NULL;            /* array of rows (triangle[i] points to T(i,·)) */
static size_t row_count = 0;               /* number of rows built (rows 0..row_count‑1) */
static size_t row_cap   = 0;               /* allocated capacity of triangle[] */

static void ensure_triangle_up_to(size_t r)
{
    if (row_count > r) return;             /* rows already built */
    if (row_cap <= r) {                    /* grow row pointer array */
        size_t new_cap = row_cap ? row_cap : 1;
        while (new_cap <= r) new_cap <<= 1;
        seq_t **tmp = (seq_t**)realloc(triangle, new_cap * sizeof(seq_t*));
        if (!tmp) exit(1);
        triangle = tmp;
        row_cap   = new_cap;
    }
    for (size_t rr = row_count; rr <= r; ++rr) {
        seq_t *row = (seq_t*)malloc((rr + 1) * sizeof(seq_t));
        if (!row) exit(1);
        if (rr == 0) {
            row[0] = (seq_t)1;
        } else if (rr == 1) {
            ensure_fib(2);                 /* need Fib(2) */
            row[0] = fib[2];
            row[1] = (seq_t)0;
        } else {                           /* rr >= 2 */
            ensure_fib(rr + 1);            /* need Fib(rr+1) */
            row[0] = fib[rr + 1];
            for (size_t k = 1; k < rr; ++k) {
                seq_t v_prev1 = triangle[rr-1][k];
                seq_t v_prev0 = triangle[rr-1][k-1];
                seq_t v_prev2 = (k <= rr-2) ? triangle[rr-2][k] : (seq_t)0;
                row[k] = v_prev1 + v_prev0 + v_prev2;
            }
            row[rr] = (seq_t)0;            /* diagonal entry */
        }
        triangle[rr] = row;
    }
    row_count = r + 1;
}

seq_t a(idx_t n)
{
    /* find row r s.t. r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    size_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= (size_t)n) r++;
    size_t k = (size_t)n - r * (r + 1) / 2;   /* column within the row */

    ensure_triangle_up_to(r);
    return triangle[r][k];
}
