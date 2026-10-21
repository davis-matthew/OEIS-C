/*
 * A209578  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209577; see the Formula section.
 * https://oeis.org/A209578
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

/* OEIS A209578: coefficients of v(n,x) derived from the recurrence
   u(1,x)=v(1,x)=1;
   u(n,x)=x*u(n-1,x)+v(n-1,x);
   v(n,x)=(x+1)*u(n-1,x)+v(n-1,x)+1.
   The sequence a(n) lists the coefficients of v(n,x) row by row
   (constant term first).  */

/* Computes the nth term of A209578.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* safety: offset is 1 */
    /* find the smallest m such that m*(m+1)/2 >= n  */
    idx_t m = 0;
    while ((m * (m + 1)) / 2 < n) ++m;

    /* allocate coefficient arrays (size m, indices 0..m-1) */
    seq_t *u = (seq_t*)calloc((size_t)m, sizeof(seq_t));
    seq_t *v = (seq_t*)calloc((size_t)m, sizeof(seq_t));
    seq_t *new_u = (seq_t*)calloc((size_t)m, sizeof(seq_t));
    seq_t *new_v = (seq_t*)calloc((size_t)m, sizeof(seq_t));
    if (!u || !v || !new_u || !new_v) { /* out of memory */
        /* fallback: return 0 (unlikely in tests) */
        return 0;
    }

    /* base row n=1 */
    u[0] = v[0] = 1;

    /* generate rows up to m */
    for (idx_t k = 2; k <= m; ++k) {
        new_u[0] = v[0];
        new_v[0] = u[0] + v[0] + 1;
        for (idx_t i = 1; i < k; ++i) {
            seq_t ui1 = u[i - 1];
            seq_t ui  = (i < k - 1) ? u[i] : 0;
            seq_t vi  = (i < k - 1) ? v[i] : 0;
            new_u[i] = ui1 + vi;
            new_v[i] = ui1 + ui + vi;
        }
        /* copy new arrays back into u and v for next iteration */
        for (size_t i = 0; i < (size_t)k; ++i) {
            u[i] = new_u[i];
            v[i] = new_v[i];
        }
    }

    /* index within the current row */
    idx_t offset = n - (m - 1) * m / 2;
    seq_t result = v[offset - 1];

    free(u);
    free(v);
    free(new_u);
    free(new_v);
    return result;
}
