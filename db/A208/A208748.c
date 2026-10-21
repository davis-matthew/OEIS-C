/*
 * A208748  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A208747; see the Formula section.
 * https://oeis.org/A208748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = T(n-1,k) + 2*T(n-1,k-1) - 2*T(n-2,k-1) + 4*T(n-2,k-2),
 *            with T(1,0)=1, T(2,0)=0, T(2,1)=4, T(3,0)=0, T(3,1)=2,
 *            T(3,2)=12, T(n,k)=0 if k<0 or k>=n. a(n) is the entries of the
 *            triangle in row‑major order
 * Range:     u8 n <= 20, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static storage for the computed triangle */
static seq_t **triangle = NULL;   /* triangle[r][k], 1‑based rows */
static idx_t  triangle_rows = 0;  /* number of rows already computed */

/* Ensure rows 1..r_needed have been computed */
static void ensure_rows(idx_t r_needed)
{
    if (triangle_rows >= r_needed)
        return;

    /* Expand row pointer array */
    seq_t **tmp = (seq_t **)realloc(triangle, sizeof(seq_t *) * (r_needed + 1));
    if (!tmp) abort();
    triangle = tmp;

    /* Compute missing rows */
    for (idx_t r = triangle_rows + 1; r <= r_needed; ++r) {
        size_t len = (size_t)r;
        seq_t *row = (seq_t *)malloc(sizeof(seq_t) * len);
        if (!row) abort();

        if (r == 1) {
            row[0] = 1;
        } else if (r == 2) {
            row[0] = 0;
            row[1] = 4;
        } else {
            seq_t *prev   = triangle[r - 1];
            seq_t *prevp  = triangle[r - 2];
            size_t lenPrev  = (size_t)(r - 1);
            size_t lenPrevp = (size_t)(r - 2);

            for (size_t k = 0; k < len; ++k) {
                seq_t val = 0;
                /* T(r-1,k) */
                if (k < lenPrev)
                    val += prev[k];
                /* 2*T(r-1,k-1) */
                if (k > 0) {
                    val += 2 * prev[k - 1];
                    /* -2*T(r-2,k-1) */
                    if (k - 1 < lenPrevp)
                        val -= 2 * prevp[k - 1];
                    /* +4*T(r-2,k-2) */
                    if (k >= 2 && k - 2 < lenPrevp)
                        val += 4 * prevp[k - 2];
                }
                row[k] = val;
            }
        }
        triangle[r] = row;
    }

    triangle_rows = r_needed;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined; return 0 for safety */

    /* Find r such that r(r-1)/2 < n ≤ r(r+1)/2 */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* Index within row r, 0‑based */
    idx_t k = n - (r * (r - 1)) / 2 - 1;
    if (k >= r)
        return 0;                    /* safety check */

    ensure_rows(r);
    return triangle[(size_t)r][(size_t)k];
}
