/*
 * A184879  Triangular array T read by rows: T(n, k) = Sum_{i=0..2*n-2*k}
 *          binomial(2*n-2*k, i)*binomial(2*k, i)*(-1)^i, 0 <= k <= n.
 * https://oeis.org/A184879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,c) where r = floor((sqrt(8*n+1)-1)/2) (row index), c
 *            = n - r*(r+1)/2 (column index within the row) and T(r,c) =
 *            Σ_{i=0}^{2*r-2*c} binomial(2*r-2*c, i)*binomial(2*c, i)*(-1)^i.
 *            This is the row‑by‑row flattening of the triangular array
 * Range:     i8 n <= 22, i16 n <= 68, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

static seq_t compute_T(idx_t r, idx_t c) {
    idx_t m1 = 2*r - 2*c;          /* 2*r - 2*c */
    idx_t m2 = 2*c;                /* 2*c     */
    idx_t max_i = m1;              /* i runs 0..2*r-2*c */
    seq_t sum = 0;                 /* cumulative sum */

    for (idx_t i = 0; i <= max_i; ++i) {
        seq_t a = binomial(m1, i); /* binom(2*r-2*c,i) */
        seq_t b = binomial(m2, i); /* binom(2*c,i)     */
        seq_t prod = a * b;        /* product of binomials */

        if (i & 1)                /* (-1)^i factor */
            sum -= prod;
        else
            sum += prod;
    }
    return sum;
}

seq_t a(idx_t n) {
    /* Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (((r + 1) * (r + 2) / 2) <= n)
        ++r;

    idx_t start_of_row = r * (r + 1) / 2;
    idx_t c = n - start_of_row;          /* column index */

    return compute_T(r, c);
}
