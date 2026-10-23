/*
 * A169658  Triangle, read by rows, defined by T(n, k) = b(n, k) + b(n, n-k+1)
 *          - (b(n,1) + b(n,n)) + 1, where b(n, k) = (-1)^n*(n!/k!)^2
 *          *binomial(n-1, k-1).
 * https://oeis.org/A169658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 16, i32 n <= 37, i64 n <= 79, i128 n <= 211,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implement sequence A169658: Triangle read by rows.
   Formula:
     T(n,k) = b(n,k) + b(n,n-k+1) - b(n,n) - b(n,1) + 1
   with
     b(n,k) = (-1)^n * (n!/k!)^2 * binomial(n-1,k-1),
   where 1 <= k <= n.
   a(n) is the nth element of the triangle read row by row. */

#include "../utils/binomial.h"

/* Compute b(n,k) as defined above. */
static seq_t
compute_b(idx_t n, idx_t k)
{
    seq_t result = (n & 1) ? (seq_t)-1 : (seq_t)1;     /* (-1)^n */

    /* Multiply by (n!/k!)^2 = product_{j=k+1}^n j^2 */
    for (idx_t j = k + 1; j <= n; ++j) {
        seq_t jj   = (seq_t)j;
        seq_t jj2  = jj * jj;   /* j^2 */
        result     = result * jj2;
    }

    /* Multiply by binomial(n-1, k-1) */
    result = result * binomial(n - 1, k - 1);
    return result;
}

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 >= n */
    idx_t r = 1;
    idx_t tri = 1;          /* triangular number T(r) = r(r+1)/2 */
    while (tri < n) {
        ++r;
        tri += r;
    }

    /* Column k (1‑based) within the row */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t k = n - prev_tri;   /* 1 <= k <= r */

    /* Compute the four b‑values */
    seq_t b_n_k      = compute_b(r, k);
    seq_t b_n_comp   = compute_b(r, r - k + 1);
    seq_t b_n_n      = compute_b(r, r);      /* (-1)^r */
    seq_t b_n_1      = compute_b(r, 1);

    /* T(r,k) = b(n,k) + b(n,n-k+1) - b(n,n) - b(n,1) + 1 */
    seq_t val = b_n_k + b_n_comp - b_n_n - b_n_1 + (seq_t)1;
    return val;
}
