/*
 * A210738  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210603; see the Formula section.
 * https://oeis.org/A210738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: generate the triangle of coefficients of v(n,x) row by row,
   storing the coefficients of v(n,x) (in increasing powers of x) in a
   flattened array so that a(n) = seq[n-1].  The triangle rows have lengths
   1,2,3,…; the recurrence for the coefficients is

   u_next[0] = v_prev[0] + 1
   u_next[i] = 2*u_prev[i-1] + v_prev[i]        (i ≥ 1)

   v_next[0] = u_prev[0] + v_prev[0] + 1
   v_next[i] = u_prev[i] + u_prev[i-1] + v_prev[i] + v_prev[i-1]
               (i ≥ 1)

   where u_prev[⋅] and v_prev[⋅] are the coefficients of
   u(n‑1,x) and v(n‑1,x).  This yields the OEIS sequence A210738.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state shared between calls */
    static seq_t *u = NULL;          /* coefficients of current u(row)   */
    static seq_t *v = NULL;          /* coefficients of current v(row)   */
    static size_t len = 0;           /* length of the current row        */
    static seq_t *seq = NULL;        /* flattened triangle               */
    static size_t seq_len = 0;       /* number of stored terms           */
    static size_t seq_cap = 0;       /* capacity of seq array            */

    /* The sequence is offset 1: a(1) is the first term. */
    if (n == 0)
        return (seq_t)0;

    /* Ensure we have computed up to term n. */
    if (seq_len < (size_t)n) {
        /* Initialise on first call. */
        if (u == NULL) {
            len = 1;
            u   = (seq_t *)malloc(sizeof(seq_t));
            v   = (seq_t *)malloc(sizeof(seq_t));
            u[0] = (seq_t)1;          /* u(1,x) = 1 */
            v[0] = (seq_t)1;          /* v(1,x) = 1 */
            seq_cap = 1;
            seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
            seq[0] = v[0];            /* first triangle entry */
            seq_len = 1;
        }

        /* Build successive rows until we reach term n. */
        while (seq_len < (size_t)n) {
            size_t next_len = len + 1;
            seq_t *u_next = (seq_t *)malloc(next_len * sizeof(seq_t));
            seq_t *v_next = (seq_t *)malloc(next_len * sizeof(seq_t));
            if (!u_next || !v_next) {
                free(u_next); free(v_next);
                return (seq_t)0; /* allocation failure */
            }

            /* Compute coefficients for the next row. */
            /* Constant term (i == 0). */
            u_next[0] = v[0] + (seq_t)1;
            v_next[0] = u[0] + v[0] + (seq_t)1;

            /* Terms for i >= 1. */
            for (size_t i = 1; i < next_len; ++i) {
                seq_t v_prev = (i < len) ? v[i] : (seq_t)0;
                seq_t u_prev_i_1 = u[i-1];
                seq_t v_prev_i_1 = v[i-1];

                /* u_next[i] = 2*u_prev[i-1] + v_prev[i] (if i < len). */
                u_next[i] = (seq_t)2 * u_prev_i_1 + v_prev;

                /* v_next[i] = u_prev[i] + u_prev[i-1] +
                            v_prev[i] + v_prev[i-1]. */
                seq_t u_prev_i   = (i < len) ? u[i] : (seq_t)0;
                seq_t v_prev_i   = v_prev; /* already set above */
                v_next[i] = u_prev_i + u_prev_i_1 + v_prev_i + v_prev_i_1;
            }

            /* Append the new row's v coefficients to the flat sequence. */
            if (seq_cap < seq_len + next_len) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                while (new_cap < seq_len + next_len)
                    new_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (!tmp) {
                    free(u_next); free(v_next);
                    return (seq_t)0; /* allocation error */
                }
                seq = tmp;
                seq_cap = new_cap;
            }
            for (size_t i = 0; i < next_len; ++i)
                seq[seq_len + i] = v_next[i];
            seq_len += next_len;

            /* Prepare for the next iteration. */
            free(u); free(v);
            u = u_next;
            v = v_next;
            len = next_len;
        }
    }

    /* Return the desired term. */
    return seq[n - 1];
}
