/*
 * A104597  Triangle T read by rows: inverse of Motzkin triangle A097609.
 * https://oeis.org/A104597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the n-th element of the triangle T read by rows, where
 *            T(n,k) is defined by the recurrence T(n,k) = T(n-1,k) +
 *            T(n-1,k-1) - T(n-2,k) - T(n-2,k-1), with base values T(0,0)=1,
 *            T(1,0)=0, T(1,1)=1, and T(n,k)=0 for k<0 or k>n. a(n) is the
 *            value of T(n,k) where n = r(r+1)/2 + k (r = row, k = c...
 * Range:     i8 n <= 79, i16 n <= 79, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#ifndef MAX_R
#define MAX_R 200   /* enough for n up to about 10 000 (row 141) */
#endif

/* Triangle storage: rows 0..MAX_R-1, columns 0..row */
static seq_t triangle[MAX_R][MAX_R] = {
    { 1 },                  /* row 0 */
    { 0, 1 }                /* row 1 */
};

static idx_t triangles_done = 1;   /* rows 0 and 1 are initialized */

/* Ensure that rows 0..rows are computed. */
static void ensure_triangle_upto(idx_t rows)
{
    if (rows <= triangles_done)
        return;

    /* Build missing rows */
    for (idx_t n = triangles_done + 1; n <= rows; ++n) {
        for (idx_t k = 0; k <= n; ++k) {
            seq_t a = (k <= n - 1) ? triangle[n - 1][k] : 0;
            seq_t b = (k >= 1)     ? triangle[n - 1][k - 1] : 0;
            seq_t c = (k <= n - 2) ? triangle[n - 2][k] : 0;
            seq_t d = (k >= 1)     ? triangle[n - 2][k - 1] : 0;
            triangle[n][k] = a + b - c - d;
        }
    }
    triangles_done = rows;
}

/* Find the row r that contains index n in the triangle
   (0‑based).  r satisfies r(r+1)/2 <= n < (r+1)(r+2)/2. */
static idx_t row_of_index(idx_t n)
{
    idx_t lo = 0, hi = MAX_R;   /* hi is exclusive upper bound */
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        idx_t start = mid * (mid + 1) / 2;
        if (start <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (idx_t)lo - 1;
}

seq_t a(idx_t n)
{
    idx_t r = row_of_index(n);
    idx_t col = n - r * (r + 1) / 2;   /* column in row r */
    ensure_triangle_upto(r);
    return triangle[r][col];
}
