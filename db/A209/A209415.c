/*
 * A209415  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209416; see the Formula section.
 * https://oeis.org/A209415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 127, u32 n <= 420, u64 n <= 1225,
 *            u128 n <= 1225, big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(n) is the flattened list of coefficients of the
 * polynomials u(n,x) defined by
 *     u(1,x)=1,  v(1,x)=1
 *     u(n,x) = x*u(n‑1,x) + v(n‑1,x)
 *     v(n,x) = (x+1)*u(n‑1,x) + x*v(n‑1,x)
 * The coefficients of u(n,x) are written row‑wise for n = 1,2,3,...
 * (row 1 has 1 coefficient, row 2 has 2, …).  a(1) = 1, a(2) = 1,
 * a(3) = 1, a(4) = 1, a(5) = 3, a(6) = 1, ...
 */

#include "../utils/seq_alloc_new_array.h"  /* provides seq_alloc_new_array */

#define MAX_ROW 256  /* enough for n up to about 10000 (row length <= 142) */

seq_t a(idx_t n)
{
    /* static cache of all flattened coefficients computed so far */
    static seq_t *flat = NULL;
    static size_t flat_len = 0;
    static size_t flat_cap = 0;

    /* static buffers holding coefficients of the previous row of u and v */
    static seq_t u_prev[MAX_ROW];
    static seq_t v_prev[MAX_ROW];
    static size_t prev_len = 0;   /* length of the previous row */

    /* initialize on first call */
    if (flat == NULL) {
        flat_cap = 64;
        flat = seq_alloc_new_array(flat_cap);

        /* row 1 */
        u_prev[0] = 1;
        v_prev[0] = 1;
        flat[0] = 1;
        flat_len = 1;
        prev_len = 1;
    }

    /* generate rows until we have at least n coefficients */
    while (flat_len < (size_t)n) {
        size_t cur_len = prev_len + 1;          /* length of new row */
        seq_t u_curr[MAX_ROW];
        seq_t v_curr[MAX_ROW];

        /* compute coefficients of the new row */
        for (size_t k = 0; k < cur_len; ++k) {
            seq_t val_u = 0;
            if (k > 0)          val_u += u_prev[k - 1];
            if (k < prev_len)   val_u += v_prev[k];
            u_curr[k] = val_u;

            seq_t val_v = 0;
            if (k < prev_len)   val_v += u_prev[k];
            if (k > 0)          val_v += u_prev[k - 1];
            if (k > 0)          val_v += v_prev[k - 1];
            v_curr[k] = val_v;
        }

        /* ensure capacity of the flattened list */
        if (flat_len + cur_len > flat_cap) {
            size_t new_cap = flat_cap ? flat_cap * 2 : 64;
            while (new_cap < flat_len + cur_len) new_cap *= 2;
            seq_t *new_flat = seq_alloc_new_array(new_cap);
            for (size_t i = 0; i < flat_len; ++i) new_flat[i] = flat[i];
            flat = new_flat;
            flat_cap = new_cap;
        }

        /* append the new row's u coefficients to the flattened list */
        for (size_t k = 0; k < cur_len; ++k) {
            flat[flat_len++] = u_curr[k];
        }

        /* copy current row to previous row buffers for next iteration */
        for (size_t k = 0; k < cur_len; ++k) {
            u_prev[k] = u_curr[k];
            v_prev[k] = v_curr[k];
        }
        prev_len = cur_len;
    }

    return flat[n - 1];
}
