/*
 * A209758  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210041; see the Formula section.
 * https://oeis.org/A209758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Triangle of coefficients of polynomials v(n,x) generated with
 *            u(1,x) = 1 , v(1,x) = 1 u(n,x) = u(n-1,x) + v(n-1,x) + 1 v(n,x)
 *            = x*u(n-1,x) + 2x*v(n-1,x) + 1 a(n) is the (n‑th) coefficient of
 *            v in row‑major order (row 1 first, then row 2, etc.). Offset is
 *            1
 * Range:     u8 n <= 32, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for n = 0 */

    /* Find the row that contains the n-th term. */
    idx_t row = 1;
    idx_t tri = 1;                 /* T(row) = row*(row+1)/2 */
    while (tri < n) {
        ++row;
        tri += row;
    }
    idx_t prev_tri = tri - row;
    idx_t offset = n - prev_tri - 1;   /* 0‑based index within the row */

    /* Arrays for u and v polynomials.  Start with row 1. */
    seq_t *u = (seq_t *)malloc(sizeof(seq_t));
    seq_t *v = (seq_t *)malloc(sizeof(seq_t));
    if (!u || !v) return 0;        /* allocation failure */
    u[0] = 1;
    v[0] = 1;
    size_t len_u = 1;    /* u length */
    size_t len_v = 1;    /* v length */

    /* Build successive rows up to 'row'. */
    for (idx_t k = 2; k <= row; ++k) {
        size_t new_len_v = (size_t)k;      /* v has k coefficients */
        size_t new_len_u = (size_t)(k - 1);/* u has k-1 coefficients */

        seq_t *v_next = (seq_t *)malloc(sizeof(seq_t) * new_len_v);
        seq_t *u_next = (seq_t *)malloc(sizeof(seq_t) * new_len_u);
        if (!v_next || !u_next) {
            free(u);
            free(v);
            return 0;                    /* allocation failure */
        }

        /* Compute v_next. */
        v_next[0] = 1;                     /* constant term */
        for (size_t d = 1; d < new_len_v; ++d) {
            seq_t uprev = (d-1 < len_u) ? u[d-1] : 0;
            seq_t vprev = (d-1 < len_v) ? v[d-1] : 0;
            v_next[d] = uprev + (vprev + vprev); /* uprev + 2*vprev */
        }

        /* Compute u_next. */
        seq_t u0 = (0 < len_u) ? u[0] : 0;
        seq_t v0 = (0 < len_v) ? v[0] : 0;
        u_next[0] = u0 + v0 + 1;
        for (size_t d = 1; d < new_len_u; ++d) {
            seq_t up = (d < len_u) ? u[d] : 0;
            seq_t vp = (d < len_v) ? v[d] : 0;
            u_next[d] = up + vp;
        }

        /* Replace old arrays. */
        free(u);
        free(v);
        u = u_next;
        v = v_next;
        len_u = new_len_u;
        len_v = new_len_v;
    }

    seq_t result = v[offset];

    /* Clean up. */
    free(u);
    free(v);
    return result;
}
