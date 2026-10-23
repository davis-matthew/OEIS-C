/*
 * A136667  Triangle read by rows: T(n, k) is the coefficient of x^k in the
 *          polynomial 1 - T_n(x)^2, where T_n(x) is the n-th Hermite
 *          polynomial of the Hochstadt kind (A137286) as related to the
 *          generalized Chebyshev in a Shabat way (A123583):
 *          p(x,n)=x*p(x,n-1)-p(x,n-2); q(x,n)=1-p(x,n)^2.
 * https://oeis.org/A136667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, tabf, sign
 * Formula:   not stated
 * Range:     i8 n <= 18, i16 n <= 51, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A136667
    Triangle T(n,k) = coefficient of x^k in 1 – H_n(x)^2,
    where the Hochstadt Hermite polynomial H_n satisfies
        H_0 = 1,
        H_1 = x,
        H_m = x*H_{m-1} – m*H_{m-2}  (m ≥ 2).
    The sequence a(n) is the triangle read by rows (row 0 has 1 entry, row 1 has 3, …),
    flattened into a single array with 1‑based indexing. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ---------- static cache for rows of H_n and flattened values ---------- */
    static seq_t **p_rows = NULL;    /* p_rows[r] points to coefficient array of H_r   */
    static size_t p_cap  = 0;         /* capacity of p_rows                           */
    static idx_t  p_len  = 0;         /* number of rows currently computed (max r+1)   */

    static seq_t *values = NULL;      /* flattened triangle values                   */
    static size_t val_cap = 0;        /* capacity of values array                    */
    static size_t val_len = 0;        /* current number of values                    */

    /* --------------------------------------------------------------------- */
    /* Return if already computed                                    */
    if (val_len >= (size_t)n)
        return values[n-1];

    /* --------------------------------------------------------------------- */
    /* Find the row that contains index n                                 */
    idx_t row_needed = 0;
    while ((row_needed + 1ULL) * (row_needed + 1ULL) < n)
        ++row_needed;    /* row_needed = floor(sqrt(n-1)) */

    /* --------------------------------------------------------------------- */
    /* Make sure we have space for the new row pointers                     */
    if (p_cap <= (size_t)row_needed) {
        size_t new_cap = p_cap ? (p_cap << 1) : 4;
        while (new_cap <= (size_t)row_needed)
            new_cap <<= 1;
        seq_t **tmp = (seq_t**)realloc(p_rows, new_cap * sizeof(seq_t*));
        p_rows = tmp;
        p_cap  = new_cap;
    }

    /* --------------------------------------------------------------------- */
    /* Generate rows up to row_needed                                      */
    for (idx_t r = p_len; r <= row_needed; ++r) {
        seq_t *cur = NULL;
        size_t r_usize = (size_t)r;            /* for array dimensions      */

        if (r == 0) {                          /* H_0 = 1                     */
            cur = (seq_t*)malloc(sizeof(seq_t));
            cur[0] = (seq_t)1;
        }
        else if (r == 1) {                     /* H_1 = x                     */
            cur = (seq_t*)malloc(2 * sizeof(seq_t));
            cur[0] = (seq_t)0;
            cur[1] = (seq_t)1;
        }
        else {                                 /* recurrence for m ≥ 2        */
            cur = (seq_t*)malloc((r_usize + 1) * sizeof(seq_t));
            /* zero-initialize */
            for (size_t i = 0; i <= r_usize; ++i)
                cur[i] = (seq_t)0;

            seq_t *prev1 = p_rows[r-1];
            seq_t *prev2 = p_rows[r-2];

            /* x * H_{r-1} term */
            for (size_t j = 0; j <= r_usize - 1; ++j)
                cur[j+1] += prev1[j];

            /* - r * H_{r-2} term */
            for (size_t j = 0; j <= r_usize - 2; ++j)
                cur[j] -= ((seq_t)r) * prev2[j];
        }

        p_rows[r] = cur;          /* store pointer to this row */

        /* ----------------------------------------------------------------- */
        /* Compute the coefficients of Q_r(x) = 1 - H_r(x)^2                 */
        size_t q_len = 2 * r_usize + 1;   /* degree 2r, so 2r+1 coefficients */

        /* Ensure enough space in the flattened array */
        if (val_cap < val_len + q_len) {
            size_t new_cap = val_cap ? (val_cap << 1) : 64;
            while (new_cap < val_len + q_len)
                new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(values, new_cap * sizeof(seq_t));
            values = tmp;
            val_cap = new_cap;
        }

        /* Compute each coefficient */
        for (size_t k = 0; k < q_len; ++k) {
            if (k == 0) {
                seq_t prod = cur[0] * cur[0];
                values[val_len++] = (seq_t)1 - prod;
            } else {
                seq_t sum = (seq_t)0;
                size_t i_start = (k > r_usize) ? k - r_usize : 0;
                size_t i_end   = (k < r_usize) ? k : r_usize;
                for (size_t i = i_start; i <= i_end; ++i)
                    sum += cur[i] * cur[k - i];
                values[val_len++] = -sum;
            }
        }
    }

    p_len = row_needed + 1;    /* now we have computed up to row_needed */

    /* --------------------------------------------------------------------- */
    /* Return the requested element                                         */
    return values[n-1];
}
