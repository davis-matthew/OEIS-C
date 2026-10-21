/*
 * A209751  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209752; see the Formula section.
 * https://oeis.org/A209751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let u1(x)=1, v1(x)=1. For n>1, u(n,x)=x*u(n-1,x)+(x+1)*v(n-1,x)
 *            v(n,x)=u(n-1,x)+2*x*v(n-1,x)+1 The triangle of coefficients of
 *            u(n,x) is flattened row by row. a(n) is the coefficient
 *            appearing in position n of that flattened triangle (rows are
 *            1‑indexed, row m has m entries). For example: 1 1 2 2...
 * Range:     u8 n <= 25, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n<1, safe guard */

    /* find row r such that T(r-1) < n <= T(r), T(m)=m*(m+1)/2 */
    idx_t r = 0;
    while (r * (r + 1) / 2 < n) r++;

    idx_t prev_tri = (r - 1) * r / 2;    /* T(r-1) */
    idx_t pos      = n - prev_tri - 1;   /* 0‑based index within the row */

    /* allocate initial row n=1 */
    seq_t *uPrev = (seq_t *)malloc(sizeof(seq_t));
    seq_t *vPrev = (seq_t *)malloc(sizeof(seq_t));
    if (!uPrev || !vPrev) return 0;     /* allocation failure guard */
    uPrev[0] = 1;
    vPrev[0] = 1;

    if (r == 1) {                       /* row 1 has only one entry */
        seq_t res = uPrev[0];
        free(uPrev);
        free(vPrev);
        return res;
    }

    /* build rows up to r */
    for (idx_t i = 2; i <= r; ++i) {
        seq_t *uNew = (seq_t *)malloc(i * sizeof(seq_t));
        seq_t *vNew = (seq_t *)malloc(i * sizeof(seq_t));
        if (!uNew || !vNew) {
            free(uPrev); free(vPrev);
            return 0;                   /* allocation failure guard */
        }

        /* initialize to zero (not strictly needed for all elements) */
        for (idx_t k = 0; k < i; ++k) {
            uNew[k] = 0;
            vNew[k] = 0;
        }

        /* u[i] = x*u[i-1] + (x+1)*v[i-1] */
        uNew[0] = vPrev[0];
        for (idx_t k = 1; k < i - 1; ++k) {
            uNew[k] = uPrev[k - 1] + vPrev[k] + vPrev[k - 1];
        }
        uNew[i - 1] = uPrev[i - 2] + vPrev[i - 2];

        /* v[i] = u[i-1] + 2x*v[i-1] + 1 */
        vNew[0] = uPrev[0] + 1;
        for (idx_t k = 1; k < i - 1; ++k) {
            vNew[k] = uPrev[k] + vPrev[k - 1] * 2;
        }
        vNew[i - 1] = vPrev[i - 2] * 2;

        /* free previous row and shift */
        free(uPrev);
        free(vPrev);
        uPrev = uNew;
        vPrev = vNew;
    }

    seq_t result = uPrev[pos];

    free(uPrev);
    free(vPrev);
    return result;
}
