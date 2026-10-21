/*
 * A209762  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209761; see the Formula section.
 * https://oeis.org/A209762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Triangle of coefficients of the polynomials v(n,x) defined by
 *  u(1,x)=v(1,x)=1 and
 *      u(n,x) = x*u(n‑1,x) + (x+1)*v(n‑1,x),
 *      v(n,x) = u(n,x) + 1.
 *
 *  The sequence a(n) is the n‑th element of that triangle read row‑by‑row.
 *  This function builds the rows one by one until the required index is
 *  reached and returns the corresponding coefficient.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)               /* not used, but safe */
        return (seq_t)0;

    /* Allocate arrays for the current row */
    seq_t *u = (seq_t *)malloc(sizeof(seq_t));
    seq_t *v = (seq_t *)malloc(sizeof(seq_t));
    if (!u || !v)
        return (seq_t)0;             /* allocation failed */

    u[0] = (seq_t)1;
    v[0] = (seq_t)1;
    size_t len = 1;                  /* current row length */

    idx_t cum = 0;                   /* count of elements processed */

    while (1)
    {
        /* Scan the current row */
        for (size_t j = 0; j < len; ++j)
        {
            ++cum;
            if (cum == n)
            {
                seq_t ans = v[j];
                free(u);
                free(v);
                return ans;
            }
        }

        /* Compute the next row */
        size_t new_len = len + 1;
        seq_t *u_next = (seq_t *)malloc(new_len * sizeof(seq_t));
        seq_t *v_next = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!u_next || !v_next)
        {
            free(u);
            free(v);
            return (seq_t)0;          /* allocation failed */
        }

        for (size_t k = 0; k < new_len; ++k)
        {
            seq_t a = (k > 0) ? u[k - 1] : (seq_t)0;
            seq_t b = (k > 0) ? v[k - 1] : (seq_t)0;
            seq_t c = (k < len) ? v[k] : (seq_t)0;
            u_next[k] = a + b + c;
            v_next[k] = u_next[k] + ((k == 0) ? (seq_t)1 : (seq_t)0);
        }

        free(u);
        free(v);
        u = u_next;
        v = v_next;
        len = new_len;
    }
}
