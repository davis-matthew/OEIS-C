/*
 * A210220  T(n, k) = -binomial(2*n-k+2, k+1)*hypergeom([2*n-k+3, 1], [k+2],
 *          2). Triangle read by rows, T(n, k) for 1 <= k <= n.
 * https://oeis.org/A210220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Series a(n) is obtained by reading the triangle T(n,k) row‑by‑row.
   T(n,k) is defined by the recurrences:
     V(1,1)=U(1,1)=1
     V(n,1)=U(n,1)=V(n-1,1)+1           (n>1)
     U(n,k)=U(n-1,k-1)+V(n-1,k)         (k>1)
     V(n,k)=U(n-1,k-1)+V(n-1,k)+V(n-1,k-1) (k>1)
   The function returns a(n) = V(row,col) for the index n. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static state across calls */
    static idx_t   a_len = 0;   /* number of cached a-values */
    static idx_t   a_cap = 0;   /* capacity of a_vals */
    static seq_t * a_vals = NULL;

    static idx_t   cur_row = 0;     /* number of rows computed so far */
    static seq_t * U_prev = NULL;   /* previous row U */
    static seq_t * V_prev = NULL;   /* previous row V */
    static seq_t * U_cur  = NULL;   /* current row U */
    static seq_t * V_cur  = NULL;   /* current row V */

    /* Ensure enough a-values are cached for the requested index n */
    while (a_len < n) {
        ++cur_row;   /* compute next row */

        /* Allocate storage for the new row */
        U_cur = (seq_t *)malloc((cur_row + 1) * sizeof(seq_t));
        V_cur = (seq_t *)malloc((cur_row + 1) * sizeof(seq_t));

        if (cur_row == 1) {
            /* Base row */
            V_cur[1] = (seq_t)1;
            U_cur[1] = (seq_t)1;
        } else {
            /* First element of the row */
            V_cur[1] = V_prev[1] + (seq_t)1;
            U_cur[1] = V_cur[1];

            /* Remaining elements */
            for (idx_t k = 2; k <= cur_row; ++k) {
                seq_t u_prev = U_prev[k - 1];
                seq_t v_prev_k   = (k <= cur_row - 1) ? V_prev[k] : (seq_t)0;
                seq_t v_prev_km1 = (k - 1 <= cur_row - 1) ? V_prev[k - 1] : (seq_t)0;

                U_cur[k] = u_prev + v_prev_k;
                V_cur[k] = u_prev + v_prev_k + v_prev_km1;
            }
        }

        /* Grow the a_vals array if necessary */
        idx_t needed = a_len + cur_row;
        if (needed > a_cap) {
            size_t newcap = a_cap ? a_cap * 2 : 64;
            while (newcap < needed) newcap <<= 1;
            a_vals = (seq_t *)realloc(a_vals, newcap * sizeof(seq_t));
            a_cap  = newcap;
        }

        /* Append the new row values to a_vals */
        for (idx_t k = 1; k <= cur_row; ++k) {
            a_vals[a_len++] = V_cur[k];
        }

        /* Free the previous row buffers */
        free(U_prev);
        free(V_prev);

        /* Prepare for the next iteration */
        U_prev = U_cur;
        V_prev = V_cur;
    }

    /* Return the cached value for the requested index */
    return a_vals[n - 1];
}
