/*
 * A210863  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210862; see the Formula section.
 * https://oeis.org/A210863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the coefficient of v(r,x), where r is the unique integer
 * satisfying T_{r-1} < n ≤ T_r (T_k = k(k+1)/2).  The polynomials u and v
 * are defined recursively by
 *
 *   u(1) = v(1) = 1
 *
 *   u(n) = u(n-1) + x·v(n-1) + 1
 *   v(n) = (x+n-1)·u(n-1) + x·v(n-1)
 *
 * The coefficient of x^k in v(n) is stored at a((n-1)n/2 + k + 1) (1‑based
 * indexing).  The function below computes this coefficient directly,
 * generating rows up to the required one each call.  No external utilities
 * are required.  All arithmetic uses only the operators and literals
 * allowed for `seq_t`.
 *
 * Initial values:
 *   a(1) = 1, a(2) = 1, a(3) = 2, a(4) = 4, a(5) = 5, a(6) = 3 …
 */

#include <stdlib.h>

/* Return the n‑th element (1‑based) of the sequence A210863.  */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined, but avoid crash */
        return (seq_t)0;

    /* Find the row r that contains the n‑th coefficient. */
    idx_t r = 1;
    idx_t sum = 1;                     /* T_r */
    while (sum < n) {                  /* find minimal r with T_r ≥ n */
        ++r;
        sum += r;
    }
    idx_t prev_sum = sum - r;          /* T_{r-1} */
    idx_t j = n - prev_sum - 1;        /* 0‑based index within row r */

    /* Allocate coefficient arrays for u and v of the current row. */
    size_t cur_size = 1;               /* size of u and v for row 1 */
    seq_t *u = (seq_t *)malloc(cur_size * sizeof(seq_t));
    seq_t *v = (seq_t *)malloc(cur_size * sizeof(seq_t));
    if (!u || !v) {                    /* allocation failed */
        if (u) free(u);
        if (v) free(v);
        return (seq_t)0;
    }

    /* Initial row n = 1: u[0] = v[0] = 1 */
    u[0] = (seq_t)1;
    v[0] = (seq_t)1;

    /* Iteratively construct rows 2 … r */
    for (idx_t row = 2; row <= r; ++row) {
        size_t new_size = (size_t)row;                /* length of next row */
        seq_t *u_next = (seq_t *)malloc(new_size * sizeof(seq_t));
        seq_t *v_next = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!u_next || !v_next) {                     /* allocation failed */
            free(u); free(v);
            if (u_next) free(u_next);
            if (v_next) free(v_next);
            return (seq_t)0;
        }

        /* i is the current row number.  i-1 is used in the recurrence. */
        seq_t i_minus_1 = (seq_t)(row - 1);

        for (size_t k = 0; k < new_size; ++k) {
            seq_t uprev = (k < cur_size) ? u[k] : (seq_t)0;          /* u_{row-1}[k] */
            seq_t bshift = (k > 0) ? v[k - 1] : (seq_t)0;           /* v_{row-1}[k-1] */

            /* u_next[k] = u_{row-1}[k] + v_{row-1}[k-1] + (k==0?1:0) */
            if (k == 0)
                u_next[k] = uprev + (seq_t)1;
            else
                u_next[k] = uprev + bshift;

            /* v_next[k] = (row-1)*uprev + u_{row-1}[k-1] + v_{row-1}[k-1] */
            seq_t part1 = i_minus_1 * uprev;
            seq_t part2 = (k > 0 && (k - 1) < cur_size) ? u[k - 1] : (seq_t)0;
            seq_t part3 = (k > 0 && (k - 1) < cur_size) ? v[k - 1] : (seq_t)0;
            v_next[k] = part1 + part2 + part3;
        }

        /* Free the previous row and move to the new one. */
        free(u);
        free(v);
        u = u_next;
        v = v_next;
        cur_size = new_size;
    }

    /* Retrieve the requested coefficient and clean up. */
    seq_t result = v[j];
    free(u);
    free(v);
    return result;
}
