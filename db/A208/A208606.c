/*
 * A208606  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208607; see the Formula section.
 * https://oeis.org/A208606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A208606: Triangle of coefficients of polynomials u(n,x)
   Recurrence:
       u_1(x)=1,           v_1(x)=1
       u_n(x)=u_{n-1}(x)+x*v_{n-1}(x)
       v_n(x)=(x+1)*u_{n-1}(x)+v_{n-1}(x)+1
   a(n) enumerates the coefficients of u_n(x) row by row.
   The implementation builds rows iteratively and returns the n'th
   coefficient. */

#include <stdlib.h>

/* O( n^2 ) algorithm, fine for n≤10 000 (≈50 M elementary ops). */
seq_t a(idx_t n)
{
    /* current row length (number of coefficients) */
    size_t len = 0;
    /* cumulative number of terms produced so far */
    idx_t cumulative = 0;

    /* Previous row data, initially NULL */
    seq_t *u = NULL;
    seq_t *v = NULL;

    while (1) {
        size_t new_len = len + 1;          /* length of next row */

        /* Allocate space for the new row (cast required for C++). */
        seq_t *next_u = (seq_t *)malloc(sizeof(seq_t) * new_len);
        seq_t *next_v = (seq_t *)malloc(sizeof(seq_t) * new_len);

        if (len == 0) {   /* u_1(x)=1, v_1(x)=1 */
            next_u[0] = 1;
            next_v[0] = 1;
        } else {
            /* Build the (len+1)-th row from the previous row. */
            for (size_t k = 0; k < new_len; ++k) {
                seq_t a_prev    = (k < len) ? u[k] : 0;
                seq_t a_prev_km = (k > 0 && k-1 < len) ? u[k-1] : 0;
                seq_t b_prev    = (k < len) ? v[k] : 0;
                seq_t b_prev_km = (k > 0 && k-1 < len) ? v[k-1] : 0;

                /* u_n(k) = u_{n-1}(k) + v_{n-1}(k-1) */
                next_u[k] = a_prev + (k > 0 ? b_prev_km : 0);

                /* v_n(k) = u_{n-1}(k) + u_{n-1}(k-1) + v_{n-1}(k) + (k==0?1:0) */
                next_v[k] = a_prev + a_prev_km + b_prev + (k == 0 ? 1 : 0);
            }
        }

        cumulative += (idx_t)new_len;  /* total terms so far */

        if (n <= cumulative) { /* answer lies in this row */
            idx_t prev_cumulative = cumulative - (idx_t)new_len;
            size_t offset = (size_t)(n - prev_cumulative); /* 1‑based within row */
            seq_t result = next_u[offset - 1];

            /* Clean up and return */
            free(u);
            free(v);
            free(next_u);
            free(next_v);
            return result;
        }

        /* Prepare for next iteration */
        free(u);
        free(v);
        u = next_u;
        v = next_v;
        len = new_len;
    }
}
