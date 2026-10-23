/*
 * A022187  Triangle of Gaussian binomial coefficients [ n,k ] for q = 23.
 * https://oeis.org/A022187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 38, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Gaussian binomial triangle (q = 23)
 *
 * T(0,0)   = 1
 * T(n,0)  = T(n,n) = 1
 * T(n,k)  = T(n-1,k-1) + 23^k * T(n-1,k)  (1 ≤ k ≤ n-1)
 *
 * The overall sequence a(n) is the row‑major flattening of this triangle:
 * a(n) = T(r,k)   where   r*(r+1)/2 ≤ n < (r+1)*(r+2)/2   and   k = n - r*(r+1)/2.
 *
 * The function below computes a(n) for any n ≥ 0 using the recurrence.
 * It is written to be valid C11 and C++17; seq_t may be a builtin integer
 * type or an arbitrary‑precision integer class.  Dynamic allocation of
 * temporary rows is performed with malloc/calloc and freed with free.
 */

#include <stdlib.h>    /* malloc, calloc, free */
#include <math.h>      /* sqrt, floor */

seq_t a(idx_t n)
{
    /* Find the row r that contains index n.  r is chosen so that
     * r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.  Use double arithmetic for the
     * initial estimate and adjust if necessary.  All arithmetic on
     * indices uses idx_t (uint64_t). */
    idx_t r;
    double d = sqrt((double)8 * n + 1.0);
    r = (idx_t)((d - 1.0) / 2.0);
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    /* Relative position within the row. */
    idx_t k = n - r * (r + 1) / 2;

    /* For n = 0 the answer is 1. */
    if (r == 0) {
        return (seq_t)1;
    }

    /* Allocate the first row (row 0). */
    seq_t *prev = (seq_t *)malloc(sizeof(seq_t));
    if (!prev) return (seq_t)0;   /* allocation failure -> 0 */
    prev[0] = (seq_t)1;

    /* Build rows up to and including row r using the recurrence. */
    for (idx_t row = 1; row <= r; ++row) {
        /* Allocate the current row of length row+1. */
        seq_t *curr = (seq_t *)calloc((size_t)(row + 1), sizeof(seq_t));
        if (!curr) {
            free(prev);
            return (seq_t)0;
        }

        curr[0] = (seq_t)1;
        curr[row] = (seq_t)1;

        seq_t power = (seq_t)1;   /* 23^col, starts with col=1 -> 23 */
        for (idx_t col = 1; col < row; ++col) {
            power = power * (seq_t)23;           /* compute 23^col */
            curr[col] = prev[col - 1] + power * prev[col];
        }

        free(prev);
        prev = curr;
    }

    /* prev now points to the desired row r. */
    seq_t result = prev[k];
    free(prev);
    return result;
}
