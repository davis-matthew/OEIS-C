/*
 * A209170  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209171; see the Formula section.
 * https://oeis.org/A209170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient of u(r,x) where r = smallest integer with
 *            r(r+1)/2 >= n, k = n - (r-1)r/2 (1‑based column within row r).
 *            u(n,x) and v(n,x) are defined by u(1,x)=1, v(1,x)=1,
 *            u(n,x)=u(n-1,x)+(x+1)*v(n-1,x),
 *            v(n,x)=(x+1)*u(n-1,x)+(x+1)*v(n-1,x)+1
 * Range:     u8 n <= 22, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first term without allocation */
    if (n == 1)
        return (seq_t)1;

    /* Find the row r and the column k (1‑based) */
    idx_t r = 1;
    idx_t total = 1;                 /* total = r(r+1)/2 */
    while (total < n) {
        ++r;
        total += r;
    }
    idx_t k = n - (r * (r - 1)) / 2;  /* 1‑based column in row r */

    /* Allocate arrays for row 1 */
    seq_t *prev_u = (seq_t *)malloc(sizeof(seq_t));
    seq_t *prev_v = (seq_t *)malloc(sizeof(seq_t));
    prev_u[0] = (seq_t)1;
    prev_v[0] = (seq_t)1;

    /* Build rows up to r */
    for (idx_t row = 2; row <= r; ++row) {
        idx_t new_len = row;
        seq_t *cur_u = (seq_t *)malloc(new_len * sizeof(seq_t));
        seq_t *cur_v = (seq_t *)malloc(new_len * sizeof(seq_t));

        for (idx_t i = 0; i < new_len; ++i) {
            /* indices from previous row */
            seq_t uprev = (i < (row - 1)) ? prev_u[i] : (seq_t)0;
            seq_t vprev = (i < (row - 1)) ? prev_v[i] : (seq_t)0;
            seq_t vprev_minus1 = (i > 0) ? prev_v[i - 1] : (seq_t)0;
            seq_t uprev_minus1 = (i > 0) ? prev_u[i - 1] : (seq_t)0;

            /* Compute new coefficients */
            cur_u[i] = uprev + vprev + vprev_minus1;
            cur_v[i] = uprev + vprev + uprev_minus1 + vprev_minus1
                       + ((i == 0) ? (seq_t)1 : (seq_t)0);
        }

        free(prev_u);
        free(prev_v);
        prev_u = cur_u;
        prev_v = cur_v;
    }

    /* Retrieve the requested coefficient (0‑based index k-1) */
    seq_t result = prev_u[k - 1];

    free(prev_u);
    free(prev_v);
    return result;
}
