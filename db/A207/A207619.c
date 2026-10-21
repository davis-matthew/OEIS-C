/*
 * A207619  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A207618; see the Formula section.
 * https://oeis.org/A207619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   computed from the recurrence u(1,x)=1, v(1,x)=1
 *            u(n,x)=u(n-1,x)+v(n-1,x) v(n,x)=x*u(n-1,x)+2x*v(n-1,x)+1 a(n) is
 *            the row‑wise enumeration of the coefficients of v(n,x) (the k‑th
 *            coefficient of v_i(x) is returned when n = 1+2+...+(i-1)+k+1, 0
 *            ≤ k < i)
 * Range:     u8 n <= 33, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"

seq_t a(idx_t n)
{
    /* static cache of rows */
    static seq_t **u_rows = NULL;
    static seq_t **v_rows = NULL;
    static size_t  rows   = 0;   /* number of rows computed so far  */
    static size_t  cap    = 0;   /* capacity of allocated row pointers */

    /* find the row i such that triangular number T_i = i*(i+1)/2 >= n */
    size_t i = rows;
    /* compute target row */
    while ((seq_t)i * (i + 1) / 2 < (seq_t)n) {
        ++i;
    }

    /* extend cache up to row i if necessary */
    if (rows < i) {
        /* enlarge pointer array if needed */
        if (cap == 0) {
            cap = 1;
            u_rows = (seq_t**)realloc(u_rows, cap * sizeof(seq_t*));
            v_rows = (seq_t**)realloc(v_rows, cap * sizeof(seq_t*));
        } else if (i > cap) {
            size_t new_cap = cap;
            while (new_cap < i) new_cap <<= 1;
            u_rows = (seq_t**)realloc(u_rows, new_cap * sizeof(seq_t*));
            v_rows = (seq_t**)realloc(v_rows, new_cap * sizeof(seq_t*));
            cap = new_cap;
        }

        /* compute rows from (rows+1) to i */
        for (size_t r = rows + 1; r <= i; ++r) {
            /* allocate coefficient arrays of length r */
            u_rows[r - 1] = alloc_seq_array(r);
            v_rows[r - 1] = alloc_seq_array(r);

            if (r == 1) {                 /* base case u1 = v1 = 1 */
                u_rows[0][0] = (seq_t)1;
                v_rows[0][0] = (seq_t)1;
                continue;
            }

            seq_t *prev_u = u_rows[r - 2];
            seq_t *prev_v = v_rows[r - 2];
            seq_t *cur_u  = u_rows[r - 1];
            seq_t *cur_v  = v_rows[r - 1];

            /* u_r = u_{r-1} + v_{r-1} (aligned by degree) */
            for (size_t k = 0; k < r; ++k) {
                seq_t sum = 0;
                if (k < r - 1) {          /* indices exist in previous row */
                    sum += prev_u[k];
                    sum += prev_v[k];
                }
                cur_u[k] = sum;
            }

            /* v_r = x*u_{r-1} + 2x*v_{r-1} + 1 */
            for (size_t k = 0; k < r; ++k) {
                seq_t val = 0;
                if (k == 0) val += (seq_t)1;           /* constant term */

                if (k >= 1) {                          /* coefficient of x^k */
                    val += prev_u[k - 1];              /* from x*u_{r-1}   */
                    seq_t tmp = prev_v[k - 1];
                    tmp += tmp;                         /* multiply by 2    */
                    val += tmp;                         /* from 2x*v_{r-1}  */
                }
                cur_v[k] = val;
            }
        }
        rows = i;
    }

    /* compute position inside the row i */
    seq_t tri_prev = (seq_t)(i - 1) * i / 2;  /* T_{i-1}   */
    size_t pos = (size_t)((seq_t)n - tri_prev - 1);

    return v_rows[i - 1][pos];
}
