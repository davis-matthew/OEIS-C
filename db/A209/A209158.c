/*
 * A209158  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209159; see the Formula section.
 * https://oeis.org/A209158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient of x^k in u(r,x), where r is the smallest
 *            integer with r(r+1)/2 ≥ n, k = n – (r-1)r/2 – 1. Recurrence
 *            (from the OEIS entry): u(1,x) = 1, v(1,x) = 1 u(n,x) = u(n-1,x)
 *            + (x+1)·v(n-1,x) v(n,x) = 2x·u(n-1,x) + x·v(n-1,x) + 1
 * Range:     u8 n <= 25, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* find the row r containing index n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    idx_t prev_tri = (r - 1) * r / 2;
    idx_t pos      = n - prev_tri - 1;   /* 0‑based position in the row */

    /* start with the first row (r = 1) */
    size_t cur_size = 1;
    seq_t *u = (seq_t *)malloc(sizeof(seq_t) * cur_size);
    seq_t *v = (seq_t *)malloc(sizeof(seq_t) * cur_size);
    if (!u || !v) { free(u); free(v); return (seq_t)0; }

    u[0] = (seq_t)1;
    v[0] = (seq_t)1;

    /* iterate to build rows 2 … r */
    for (idx_t step = 2; step <= r; ++step) {
        size_t new_size = cur_size + 1;

        seq_t *u_new = (seq_t *)malloc(sizeof(seq_t) * new_size);
        seq_t *v_new = (seq_t *)malloc(sizeof(seq_t) * new_size);
        if (!u_new || !v_new) {
            free(u); free(v);
            free(u_new); free(v_new);
            return (seq_t)0;
        }

        /* v_new[0] = 1 */
        v_new[0] = (seq_t)1;

        /* compute the rest of v_new */
        for (size_t i = 1; i < new_size; ++i)
            v_new[i] = (seq_t)2 * u[i - 1] + v[i - 1];

        /* u_new[0] = u[0] + v[0] */
        u_new[0] = u[0] + v[0];

        /* compute the rest of u_new */
        for (size_t i = 1; i < new_size; ++i) {
            seq_t term_u = (i < cur_size) ? u[i] : (seq_t)0;
            seq_t term_v = (i < cur_size) ? v[i] : (seq_t)0;
            u_new[i] = term_u + term_v + v[i - 1];
        }

        /* free old arrays and update pointers */
        free(u);
        free(v);
        u = u_new;
        v = v_new;
        cur_size = new_size;
    }

    /* the desired coefficient is now stored in u[pos] */
    seq_t result = u[pos];

    free(u);
    free(v);

    return result;
}
