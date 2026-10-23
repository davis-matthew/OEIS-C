/*
 * A137372  Triangle read by rows: coefficients of Fermat-Lucas polynomials.
 * https://oeis.org/A137372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 17, i16 n <= 51, i32 n <= 182, i64 n <= 689,
 *            i128 n <= 1274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Sequence of coefficients of Fermat‑Lucas polynomials.
   Let c[n][k] be the coefficient of y^k in F(n).  The polynomials are
   defined by
        F(0) = 2,
        F(1) = 3*y,
        F(n) = 3*y*F(n-1) - 2*F(n-2)    for n > 1.
   The coefficient satisfies
        c[0][0] = 2,
        c[1][0] = 0,  c[1][1] = 3,
        c[n][k] = 3*c[n-1][k-1] - 2*c[n-2][k]      (out‑of‑range indices are 0).
   The flattened sequence a(n) is obtained by reading each row
   left to right:
        a(0) = c[0][0],
        a(1) = c[1][0],  a(2) = c[1][1],
        a(3) = c[2][0],  a(4) = c[2][1],  a(5) = c[2][2], …   */

/* Cached rows of coefficients. */
static seq_t **poly = NULL;
static idx_t  max_row = 0;

/* Ensure rows 0 … r are computed and stored in poly[]. */
static void ensure_rows(idx_t r)
{
    if (r < max_row) return;
    seq_t **new_poly = (seq_t**)realloc(poly, (r+1) * sizeof(seq_t*));
    if (!new_poly) abort();          /* allocation failure */
    poly = new_poly;

    for (idx_t n = max_row; n <= r; ++n)
    {
        poly[n] = (seq_t*)calloc(n+1, sizeof(seq_t));
        if (!poly[n]) abort();

        if (n == 0)                 /* original definition */
        {
            poly[0][0] = (seq_t)2;
            max_row = 1;
            continue;
        }
        if (n == 1)                 /* original definition */
        {
            poly[1][0] = (seq_t)0;
            poly[1][1] = (seq_t)3;
            max_row = 2;
            continue;
        }

        /* Compute row n from rows n‑1 and n‑2. */
        for (idx_t k = 0; k <= n; ++k)
        {
            seq_t t1 = 0;
            if (k > 0) {
                t1 = poly[n-1][k-1];
                t1 *= (seq_t)3;
            }
            seq_t t2 = 0;
            if (k <= n-2) {
                t2 = poly[n-2][k];
                t2 *= (seq_t)2;
            }
            poly[n][k] = t1 - t2;
        }
        max_row = n+1;
    }
}

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r+1)*(r+2)/2 <= n) r++;
    idx_t pre = r*(r+1)/2;
    idx_t k   = n - pre;              /* column index within the row */

    ensure_rows(r);                   /* compute up to required row */
    return poly[r][k];
}
