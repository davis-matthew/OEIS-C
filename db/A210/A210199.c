/*
 * A210199  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210200; see the Formula section.
 * https://oeis.org/A210199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = coefficient of x^(pos-1) in u(row) where u(1,x)=1,
 *            v(1,x)=1, u(n,x)=u(n-1,x)+v(n-1,x)+1,
 *            v(n,x)=(x+1)u(n-1,x)+x*v(n-1,x)+1. This generates the triangle
 *            of coefficients of u(n,x)
 * Range:     u8 n <= 30, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   add_row, pos_in_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/add_row.h"
#include "../utils/pos_in_row.h"

/*------------------------------------------------------------*/
static inline idx_t row_from_index(idx_t n)
{
    /* Find smallest r such that 1 + r(r-1)/2 >= n  (C(r) >= n) */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t cum = 1 + mid * (mid - 1) / 2;   /* safe for 64‑bit */
        if (cum >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined, but harmless */

    /* Determine which row and position within that row. */
    idx_t row = row_from_index(n);
    idx_t pos  = pos_in_row(n, row);   /* 1‑based */

    /* Start with row 1: u(1,x)=1, v(1,x)=1 */
    seq_t *u_cur = (seq_t*)malloc(sizeof(seq_t));
    seq_t *v_cur = (seq_t*)malloc(sizeof(seq_t));
    *u_cur = 1;
    *v_cur = 1;
    idx_t len_u = 1, len_v = 1;

    if (row > 1) {
        add_row(row, &u_cur, &len_u, &v_cur, &len_v);
    }

    /* Extract the required coefficient.  For row 1 pos will always be 1. */
    seq_t result = u_cur[pos - 1];

    /* Clean up */
    free(u_cur);
    free(v_cur);

    return result;
}
