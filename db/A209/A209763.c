/*
 * A209763  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209764; see the Formula section.
 * https://oeis.org/A209763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   u(1,x) = 1, v(1,x) = 1 u(n,x) = x*u(n-1,x) + (x+1)*v(n-1,x)
 *            v(n,x) = 2x*u(n-1,x) + v(n-1,x) + 1 The sequence A209763 is the
 *            flattened triangle of coefficients of u(n,x)
 * Range:     u8 n <= 25, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1
 * a(2) = 1, a(3) = 2
 * a(4) = 2, a(5) = 5, a(6) = 4
 * ... */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static state for lazy incremental generation */
    static idx_t     max_row   = 0;      /* highest row already built */
    static seq_t    *flat      = NULL;   /* flattened coefficients of all rows */
    static size_t    flat_cap  = 0;      /* capacity of flat */
    static size_t    flat_len  = 0;      /* number of elements in flat */

    static seq_t   *u_prev    = NULL;   /* coefficients of u(prev_row,x) */
    static seq_t   *v_prev    = NULL;   /* coefficients of v(prev_row,x) */
    static idx_t    prev_len  = 0;      /* length of previous row (=max_row) */

    /* Find the row that contains the requested index n.
       Row r satisfies r(r+1)/2 >= n > r(r-1)/2.  */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) {
        ++r;
    }

    /* Build rows up to r if they are not already present. */
    if (r > max_row) {
        /* Initial row (row 1) if not yet built */
        if (max_row == 0) {
            prev_len = 1;
            u_prev = (seq_t *)calloc(prev_len, sizeof(seq_t));
            v_prev = (seq_t *)calloc(prev_len, sizeof(seq_t));
            if (!u_prev || !v_prev) abort();
            u_prev[0] = (seq_t)1;
            v_prev[0] = (seq_t)1;

            /* Append coefficients of u(1) to the flat array */
            if (flat_len + prev_len > flat_cap) {
                size_t new_cap = (flat_cap ? flat_cap * 2 : 16);
                seq_t *tmp = (seq_t *)realloc(flat, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                flat      = tmp;
                flat_cap  = new_cap;
            }
            flat[flat_len++] = u_prev[0];
            max_row = 1;
        }

        /* Generate subsequent rows */
        for (idx_t row = max_row + 1; row <= r; ++row) {
            idx_t cur_len = row;          /* coefficients of u(row,x) */
            seq_t *u_cur = (seq_t *)calloc(cur_len, sizeof(seq_t));
            seq_t *v_cur = (seq_t *)calloc(cur_len, sizeof(seq_t));
            if (!u_cur || !v_cur) abort();

            /* Build the polynomials for this row */
            for (idx_t i = 0; i < prev_len; ++i) {
                /* u(row,x) = x*u_prev + (x+1)*v_prev */
                u_cur[i + 1] += u_prev[i];          /* x*u_prev */
                u_cur[i + 1] += v_prev[i];          /* x*v_prev  */
                u_cur[i]     += v_prev[i];          /* +v_prev   */

                /* v(row,x) = 2x*u_prev + v_prev + 1 */
                v_cur[i + 1] += (seq_t)2 * u_prev[i]; /* 2x*u_prev */
                v_cur[i]     += v_prev[i];            /* +v_prev   */
            }
            v_cur[0] += (seq_t)1;               /* +1 constant term */

            /* Append u(row) coefficients to the flat array */
            if (flat_len + cur_len > flat_cap) {
                size_t new_cap = (flat_cap ? flat_cap * 2 : 16);
                seq_t *tmp = (seq_t *)realloc(flat, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                flat      = tmp;
                flat_cap  = new_cap;
            }
            for (idx_t i = 0; i < cur_len; ++i) {
                flat[flat_len++] = u_cur[i];
            }

            /* Prepare for next iteration */
            free(u_prev);
            free(v_prev);
            u_prev = u_cur;
            v_prev = v_cur;
            prev_len = cur_len;
            max_row = row;
        }
    }

    /* Return the requested term from the flattened array. */
    return flat[n - 1];
}
