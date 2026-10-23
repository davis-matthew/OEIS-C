/*
 * A104035  Triangle T(n,k), 0 <= k <= n, read by rows, defined by T(0,0) = 1;
 *          T(0,k) = 0 if k>0 or if k<0; T(n,k) = k*T(n-1,k-1) +
 *          (k+1)*T(n-1,k+1).
 * https://oeis.org/A104035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl, nice
 * Formula:   a(n) is the triangle entry T(row,col) where row =
 *            row_of_index(n) and col = n - row*(row+1)/2. The triangle T(n,k)
 *            is defined by T(0,0) = 1; T(0,k) = 0 for k>0 or k<0; T(n,k) =
 *            k*T(n-1,k-1) + (k+1)*T(n-1,k+1) for n>0, 0<=k<=n
 * Range:     u8 n <= 22, u16 n <= 39, u32 n <= 95, u64 n <= 219,
 *            u128 n <= 573, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAX_ROW 200   /* satisfies n <= 10000 (row ~ 141) */

static bool triangle_built = false;
static seq_t triangle[MAX_ROW][MAX_ROW];

/* Build the whole triangle up to MAX_ROW-1 once */
static void build_triangle(void)
{
    if (triangle_built) return;
    /* initialize all to zero */
    for (idx_t i = 0; i < MAX_ROW; ++i)
        for (idx_t j = 0; j < MAX_ROW; ++j)
            triangle[i][j] = 0;

    triangle[0][0] = 1;

    for (idx_t n = 1; n < MAX_ROW; ++n) {
        for (idx_t k = 0; k <= n; ++k) {
            seq_t val = 0;
            if (k > 0) /* k*T(n-1,k-1) */
                val += (seq_t)k * triangle[n-1][k-1];
            if (k + 1 <= n - 1) /* (k+1)*T(n-1,k+1) */
                val += (seq_t)(k + 1) * triangle[n-1][k+1];
            triangle[n][k] = val;
        }
    }
    triangle_built = true;
}

/* Compute the row number r such that r*(r+1)/2 <= idx < (r+1)*(r+2)/2 */
static idx_t row_of_index(idx_t idx)
{
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= idx)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    build_triangle();

    idx_t r = row_of_index(n);
    idx_t c = n - r * (r + 1) / 2;

    if (r >= MAX_ROW || c > r)
        return 0; /* safety, though n should be within limits */

    return triangle[r][c];
}
