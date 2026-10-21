/*
 * A209819  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209820; see the Formula section.
 * https://oeis.org/A209819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the coefficient of x^k in the polynomial u(r,x)
 *            generated * by the recurrence * u(1,x)=v(1,x)=1 * u(n,x) =
 *            x*u(n-1,x) + 2x*v(n-1,x) + 1 * v(n,x) = (x+1)*u(n-1,x) +
 *            x*v(n-1,x) + 1 * for the unique r and k with * r(r-1)/2 < n ≤
 *            r(r+1)/2 and k = n - r(r-1)/2 - 1
 * Range:     u8 n <= 25, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;  /* undefined for n<1 */

    /* determine row r by increasing until r(r+1)/2 >= n */
    idx_t r = 1;
    while ((unsigned __int128)r * (r + 1) / 2 < n) ++r;

    idx_t tot_prev = (idx_t)((unsigned __int128)r * (r - 1) / 2);
    idx_t k = n - tot_prev - 1;                 /* 0‑based power index */

    /* allocate arrays for two successive rows */
    size_t len = (size_t)r;
    seq_t *prev_u = (seq_t *)malloc(len * sizeof(seq_t));
    seq_t *prev_v = (seq_t *)malloc(len * sizeof(seq_t));
    seq_t *tmp_u  = (seq_t *)malloc(len * sizeof(seq_t));
    seq_t *tmp_v  = (seq_t *)malloc(len * sizeof(seq_t));

    if (!prev_u || !prev_v || !tmp_u || !tmp_v) {
        /* allocation failed, return 0 as fallback */
        free(prev_u); free(prev_v); free(tmp_u); free(tmp_v);
        return (seq_t)0;
    }

    /* initial row (n = 1) */
    prev_u[0] = (seq_t)1;
    prev_v[0] = (seq_t)1;

    /* compute rows 2 .. r */
    for (idx_t cur = 2; cur <= r; ++cur) {
        tmp_u[0] = (seq_t)1;
        tmp_v[0] = prev_u[0] + (seq_t)1;

        for (idx_t idx = 1; idx < cur; ++idx) {
            seq_t pu = prev_u[idx - 1];
            seq_t pv = prev_v[idx - 1];

            /* u(n,x) coefficient */
            tmp_u[idx] = pu + ((seq_t)2 * pv);

            /* v(n,x) coefficient */
            seq_t pu_next = (idx < cur - 1) ? prev_u[idx] : (seq_t)0;
            tmp_v[idx] = pu_next + pu + pv;
        }

        /* swap prev and tmp for next iteration */
        seq_t *swap = prev_u; prev_u = tmp_u; tmp_u = swap;
        swap = prev_v;        prev_v = tmp_v;     tmp_v = swap;
    }

    /* result is coefficient k of the final row */
    seq_t result = prev_u[k];

    free(prev_u);
    free(prev_v);
    free(tmp_u);
    free(tmp_v);

    return result;
}
