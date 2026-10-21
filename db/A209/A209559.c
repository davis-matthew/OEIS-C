/*
 * A209559  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209560; see the Formula section.
 * https://oeis.org/A209559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle of coefficients of polynomials u(n,x) defined by
   u(1,x) = 1,  v(1,x) = 1
   u(n,x) = x*u(n-1,x) + v(n-1,x)
   v(n,x) = u(n-1,x) + (x+1)*v(n-1,x) + 1
   The array a(n) is the row‑wise flattening of the coefficients
   of u(n,x) (constant term first, highest degree last).  For a given n
   we locate the unique row r with r*(r+1)/2 ≥ n, and p = n - (r-1)*r/2
   gives the position inside that row (1‑based).  The return value is
   the coefficient of x^(p-1) in u(r,x).  All arithmetic is performed on
   type seq_t, which may be any unsigned integer type or an arbitrary‑precision
   integer class. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* locate row r and position p */
    idx_t r = 1;
    idx_t cum = 1;                 /* cumulative entries up to current row */
    while (cum < n) {
        r++;
        cum += r;
    }
    idx_t p = n - (cum - r);       /* 1‑based position inside the row */

    /* first row is trivial */
    if (r == 1)
        return 1;

    /* allocate coefficient arrays */
    seq_t *u  = (seq_t *)malloc(sizeof(seq_t) * r);
    seq_t *v  = (seq_t *)malloc(sizeof(seq_t) * r);
    seq_t *nu = (seq_t *)malloc(sizeof(seq_t) * r);   /* next u */
    seq_t *nv = (seq_t *)malloc(sizeof(seq_t) * r);   /* next v */
    if (!u || !v || !nu || !nv)
        return 0;                 /* memory allocation failure */

    /* initialize for n = 1 */
    for (idx_t i = 0; i < r; ++i) {
        u[i]  = 0;
        v[i]  = 0;
        nu[i] = 0;
        nv[i] = 0;
    }
    u[0] = 1;
    v[0] = 1;

    /* build successive rows up to r */
    for (idx_t cur = 2; cur <= r; ++cur) {
        /* clear next arrays for indices 0..cur-1 */
        for (idx_t i = 0; i < cur; ++i) {
            nu[i] = 0;
            nv[i] = 0;
        }

        /* new_u = x*u + v  */
        for (idx_t i = 0; i < cur; ++i) {
            if (i > 0)
                nu[i] += u[i - 1];
            nu[i] += v[i];
        }

        /* new_v = u + (x+1)*v + 1 */
        nv[0] = v[0] + u[0] + 1;
        for (idx_t i = 1; i < cur; ++i) {
            nv[i] = u[i] + v[i] + v[i - 1];
        }

        /* swap pointers */
        seq_t *tmp = u;  u = nu;  nu = tmp;
        tmp = v;        v = nv;  nv = tmp;

        /* zero out excess elements in the old arrays */
        for (idx_t i = cur; i < r; ++i) {
            nu[i] = 0;
            nv[i] = 0;
        }
    }

    /* retrieve the requested coefficient */
    seq_t result = u[p - 1];

    /* free allocated memory */
    free(u);
    free(v);
    free(nu);
    free(nv);

    return result;
}
