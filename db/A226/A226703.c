/*
 * A226703  Triangle read by rows: T(n,k) =
 *          binomial(2*n,k)*Stirling2(2*n-k,n).
 * https://oeis.org/A226703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A226703:
   a(n) = T(r,k) where r = max{m | m*(m+1)/2 ≤ n}
           and k = n - r*(r+1)/2.
   T(r,k) = binomial(2*r,k) * Stirling2(2*r - k, r).          */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"          /* seq_t binomial(idx_t n, idx_t k); */

/* Compute Stirling numbers of the second kind S(m,k) for 0 ≤ k ≤ m
   using the recurrence: S(m,k) = k*S(m-1,k) + S(m-1,k-1).        */
static seq_t stirling2(idx_t m, idx_t k)
{
    if (k == 0)            return (m == 0) ? (seq_t)1 : (seq_t)0;
    if (k > m)             return (seq_t)0;

    /* Allocate two rows of length k+1. */
    size_t len = (size_t)k + 1;
    seq_t *prev = (seq_t *)malloc(len * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc(len * sizeof(seq_t));
    if (!prev || !curr) {
        /* Out‑of‑memory fall‑back: return 0 (not expected for small n). */
        free(prev); free(curr);
        return (seq_t)0;
    }

    /* Initialize S(0,0)=1, S(0,j)=0 for j>0. */
    for (size_t i = 0; i <= k; ++i) prev[i] = (i == 0) ? (seq_t)1 : (seq_t)0;

    for (idx_t row = 1; row <= m; ++row) {
        curr[0] = (seq_t)0;
        size_t upto = (row < (idx_t)k) ? (size_t)row : (size_t)k;
        for (size_t j = 1; j <= upto; ++j) {
            seq_t prod = prev[j] * (seq_t)j;      /* j * prev[j] */
            curr[j] = prod + prev[j - 1];         /* + prev[j-1]   */
        }
        for (size_t j = upto + 1; j <= k; ++j) curr[j] = (seq_t)0;

        /* Swap rows for next iteration. */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[k];
    free(prev);
    free(curr);
    return result;
}

extern seq_t a(idx_t n)
{
    /* Find r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 by incremental summation. */
    idx_t r = 0;
    idx_t tri = 0;                     /* current triangular number r*(r+1)/2 */
    while (tri + r + 1 <= n) {
        ++r;
        tri += r;
    }

    idx_t k = n - tri;                 /* column index in row r */

    seq_t bin = binomial((idx_t)(2 * r), k);
    seq_t st  = stirling2((idx_t)((2 * r) - k), r);

    return bin * st;
}
