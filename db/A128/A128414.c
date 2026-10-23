/*
 * A128414  Riordan array ((1-2x)/(1+2x),x/(1+2x)^2).
 * https://oeis.org/A128414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = T(r,k) where r = largest integer such that r(r+1)/2 ≤ n
 *            (row index) k = n - r(r+1)/2 (column index) The triangle T is
 *            defined by T(0,0) = 1 T(1,0) = -4 , T(1,1) = 1 T(2,0) = 8 ,
 *            T(2,1) = -8 , T(2,2) = 1 for n ≥ 3, 0 ≤ k ≤ n T(n,k) =
 *            T(n‑1,k‑1) - 4*T(n‑1,k) - 4*T(n‑2,k) with T(n,k)=0 if k<0...
 * Range:     i8 n <= 15, i16 n <= 41, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   offset_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/offset_row.h"

/* 1‑D storage of the triangle: row r starts at index r(r+1)/2   */
static seq_t *tri = NULL;
static idx_t currentMaxRow = 0;          /* highest row already computed */
static int initialized = 0;              /* 0 = not yet initialized */

/* linear offset of the first element of row r */

/* build the triangle up to row `need_row` inclusive */
static void ensure_cache(idx_t need_row)
{
    /* always need space for rows 0…2, even if need_row < 2 */
    idx_t req = (need_row > 2) ? need_row : 2;
    idx_t newSize = (req + 1) * (req + 2) / 2;          /* total elements up to row req */

    seq_t *newTri = (seq_t*)realloc(tri, newSize * sizeof(seq_t));
    if (!newTri) return;               /* memory failure – abort */
    tri = newTri;

    if (!initialized) {
        /* initialize the first three rows */
        tri[0] = (seq_t)1;                     /* T(0,0) */

        tri[1] = (seq_t)-4;                    /* T(1,0) */
        tri[2] = (seq_t)1;                     /* T(1,1) */

        tri[3] = (seq_t)8;                     /* T(2,0) */
        tri[4] = (seq_t)-8;                    /* T(2,1) */
        tri[5] = (seq_t)1;                     /* T(2,2) */

        currentMaxRow = 2;
        initialized = 1;
    }

    /* compute remaining rows, if any */
    for (idx_t n = currentMaxRow + 1; n <= need_row; ++n) {
        idx_t offset     = offset_row(n);
        idx_t offsetPrev = offset_row(n - 1);
        idx_t offsetPrev2 = (n >= 2) ? offset_row(n - 2) : 0;

        for (idx_t k = 0; k <= n; ++k) {
            seq_t t1 = (k > 0)          ? tri[offsetPrev + k - 1] : (seq_t)0;
            seq_t t2 = (k <= n - 1)     ? tri[offsetPrev + k]     : (seq_t)0;
            seq_t t3 = (n >= 2 && k <= n - 2) ? tri[offsetPrev2 + k] : (seq_t)0;
            tri[offset + k] = t1 - (seq_t)4 * t2 - (seq_t)4 * t3;
        }
        currentMaxRow = n;
    }
}

/* required public function */
seq_t a(idx_t n)
{
    /* find row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t k = n - r * (r + 1) / 2;   /* column within the row */
    ensure_cache(r);
    return tri[offset_row(r) + k];
}
