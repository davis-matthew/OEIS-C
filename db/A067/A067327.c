/*
 * A067327  Triangle related to generalized Catalan numbers A064340.
 * https://oeis.org/A067327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the n-th element of a triangular array defined by
 *
 *   a(n,0) = C(2,2; n)                           (Generalised Catalan)
 *   a(n,1) = 3*C(2,2; n)   for n ≥ 1
 *   a(n,m) = 4*Σ_{k=m-1}^{n-1} a(n-1,k)  for n ≥ m ≥ 2   else 0
 *
 * Rows are flattened in row‑major order:
 *   a(0)=a(0,0), a(1)=a(1,0), a(2)=a(1,1), a(3)=a(2,0), …
 *
 * The function below computes a(n) on demand and caches whole rows
 * so that repeated calls are fast.
 */

#include <stdlib.h>
#include <math.h>

static seq_t **triangle = NULL;          /* array of pointers to each row */
static size_t  tri_cap   = 0;            /* allocated capacity for triangle */
static size_t  tri_len   = 0;            /* number of rows currently stored */

static seq_t *row_sums = NULL;          /* sum of each row, used to build next row */
static size_t  sum_cap   = 0;
static size_t  sum_len   = 0;

/* Ensure that the storage for 'len' rows exists, growing if necessary */
static void grow_triangle(size_t len)
{
    if (len <= tri_cap) return;
    size_t new_cap = tri_cap ? tri_cap << 1 : 4;
    while (new_cap < len) new_cap <<= 1;
    triangle = (seq_t**)realloc(triangle, new_cap * sizeof(seq_t*));
    tri_cap = new_cap;
}

static void grow_sums(size_t len)
{
    if (len <= sum_cap) return;
    size_t new_cap = sum_cap ? sum_cap << 1 : 4;
    while (new_cap < len) new_cap <<= 1;
    row_sums = (seq_t*)realloc(row_sums, new_cap * sizeof(seq_t));
    sum_cap = new_cap;
}

/* Build rows incrementally up to and including row 'idx' (0‑based) */
static void ensure_rows(idx_t idx)
{
    while (tri_len <= idx) {
        size_t i = tri_len;          /* index of next row to build */

        grow_triangle(i + 1);
        grow_sums(i + 1);

        if (i == 0) {                 /* first row */
            triangle[0] = (seq_t*)malloc(sizeof(seq_t));
            triangle[0][0] = (seq_t)1;
            row_sums[0] = (seq_t)1;
        } else {
            size_t prev_len = i;                     /* previous row length = i */
            seq_t *prev   = triangle[i - 1];
            seq_t *curr   = (seq_t*)malloc((i + 1) * sizeof(seq_t));
            seq_t row_sum = (seq_t)0;

            /* m = 0 */
            curr[0] = row_sums[i - 1];   /* sum of previous row */
            row_sum += curr[0];

            /* m = 1 (only for i≥1) */
            if (i >= 1) {
                curr[1] = (seq_t)3 * curr[0];
                row_sum += curr[1];
            }

            /* m ≥ 2 */
            for (size_t m = 2; m <= i; ++m) {
                seq_t s = (seq_t)0;
                for (size_t k = m - 1; k < prev_len; ++k) s += prev[k];
                curr[m] = (seq_t)4 * s;
                row_sum += curr[m];
            }

            triangle[i] = curr;
            row_sums[i] = row_sum;
        }
        ++tri_len;
        ++sum_len;
    }
}

/* Compute the row number 'r' such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
static idx_t compute_row(idx_t n)
{
    /* Initial estimate using double sqrt */
    idx_t r = (idx_t)((sqrt((double)(8 * n + 1)) - 1) / 2);

    /* Correct any rounding errors */
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n) ++r;
    while ((unsigned __int128)r * (r + 1) / 2 > n)         --r;
    return r;
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    idx_t r = compute_row(n);
    idx_t m = n - (idx_t)((unsigned __int128)r * (r + 1) / 2);

    ensure_rows(r);
    return triangle[r][m];
}
