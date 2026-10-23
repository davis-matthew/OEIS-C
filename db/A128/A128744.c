/*
 * A128744  Triangle read by rows: T(n,k) is the number of skew Dyck paths of
 *          semilength n and having height of the first peak equal to k (1 <=
 *          k <= n).
 * https://oeis.org/A128744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn
 * Formula:   a(n) = Flattened triangle T(r,k) where r(r+1)/2 ≥ n, T(r,1) =
 *            T(r,2) = A002212(r-1) for r ≥ 3, and for r ≥ 3, k ≥ 3: T(r,k) =
 *            T(r-1,k-1) + Σ_{i=0}^{r-k} A002212(i) * T(r-1-i,k-1)
 * Range:     u8 n <= 21, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We compute the triangle incrementally.
   A002212 sequence g[0..] satisfies:
     g[0] = 1, g[1] = 1,
     g[n] = g[n-1] + Σ_{i=0}^{n-1} g[i] * g[n-1-i]  (for n ≥ 2)   */

#include <stdlib.h>
#include <math.h>

/* Types are supplied by the build system.  Do not #define them. */
static seq_t *g = NULL;           /* holds A002212 sequence, index 0.. */
static idx_t g_size = 0;          /* number of g values currently stored */

static seq_t **triangle = NULL;   /* triangle[r] -> array of length r (1-based) */
static idx_t triangle_rows = 0;   /* number of rows currently stored */

static void ensure_g(idx_t size_needed) {
    if (g_size >= size_needed) return;
    seq_t *new_g = (seq_t *)realloc(g, sizeof(seq_t) * size_needed);
    if (!new_g) abort();          /* allocation error */
    g = new_g;
    /* initialise new elements */
    if (g_size == 0) {
        g[0] = 1;
        g_size = 1;
        if (size_needed == 1) return;
        g[1] = 1;
        g_size = 2;
    }
    for (idx_t n = g_size; n < size_needed; ++n) {
        seq_t sum = 0;
        for (idx_t i = 0; i <= n-1; ++i) {
            sum += g[i] * g[n-1-i];
        }
        g[n] = g[n-1] + sum;
    }
    g_size = size_needed;
}

static void ensure_triangle(idx_t rows_needed) {
    if (triangle_rows >= rows_needed) return;
    /* allocate/resize array of row pointers */
    seq_t **new_rows = (seq_t **)realloc(triangle, sizeof(seq_t *) * (rows_needed + 1));
    if (!new_rows) abort();
    triangle = new_rows;
    /* compute missing rows */
    for (idx_t r = triangle_rows + 1; r <= rows_needed; ++r) {
        triangle[r] = (seq_t *)malloc(sizeof(seq_t) * r);
        if (!triangle[r]) abort();
        /* row r has indices 1..r, stored at 0..r-1 */
        if (r == 1) {
            triangle[1][0] = 1;                /* T(1,1) */
        } else {
            /* Ensure g up to r-1 */
            ensure_g(r); 
            /* T(r,1) */
            triangle[r][0] = g[r-1];
            /* T(r,2) */
            if (r == 2) {
                triangle[r][1] = 2;            /* special case */
            } else {
                triangle[r][1] = g[r-1];
            }
            /* Remaining columns */
            for (idx_t k = 3; k <= r; ++k) {
                idx_t i_max = r - k;           /* number of terms minus 1 */
                seq_t sum = 0;
                for (idx_t i = 0; i <= i_max; ++i) {
                    sum += g[i] * triangle[r - 1 - i][k - 2];
                }
                triangle[r][k-1] = triangle[r-1][k-2] + sum;
            }
        }
    }
    triangle_rows = rows_needed;
}

static inline idx_t offset_row(idx_t r) {
    /* number of terms before row r (r starts at 1) */
    return (r * (r - 1)) / 2;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    /* find the row r such that offset_row(r) < n <= offset_row(r+1) */
    long double d = sqrt((long double)8.0 * n + 1.0);
    idx_t r = (idx_t)((d - 1.0) / 2.0 + 1.0);
    while (offset_row(r) >= n) --r;
    while (offset_row(r+1) < n) ++r;
    idx_t k = n - offset_row(r);              /* 1‑based column */
    ensure_triangle(r);
    return triangle[r][k-1];
}
