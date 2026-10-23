/*
 * A144332  Triangle read by rows, A144328 * A007318.
 * https://oeis.org/A144332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (row, column) entry of the triangle defined by
 * Range:     u8 n <= 30, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(r,j) = sum_{k=1}^{r-1} k * C(k, j-1)  (for j ≥ 2)   */
 /* a(r,1) = 1 + sum_{k=1}^{r-1} k * C(k,0) = 1 + r(r-1)/2 */
 /* where C(k, m) are binomial coefficients (Pascal's triangle). */

#include <stdint.h>
#include "../utils/binom.h"   /* provides: seq_t binom(idx_t a, idx_t b); */

static inline idx_t triangular(idx_t n)
{
    /* n*(n+1)/2  (uses uint64_t type for n; safe for n ≤ 10^6) */
    return n * (n + 1) / 2;
}

/* ------------------------------------------------------------ */
/* Computes the n-th term (1‑based) of OEIS A144332                */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Find row r such that triangular(r-1) < n ≤ triangular(r) */
    idx_t r = 0;
    while (triangular(r) < n)
        ++r;

    /* Column j in that row: 1‑based */
    idx_t j = n - triangular(r - 1);

    seq_t sum = 0;

    /* Sum over k = 1 … r‑1 : k * C(k, j‑1) */
    for (idx_t k = 1; k <= r - 1; ++k)
    {
        /* binom returns 0 if j‑1 > k, so we may skip the check to keep code short */
        seq_t term = binom(k, j - 1);
        term = term * (seq_t)k;   /* k is small (≤ r ≤ 200) */
        sum += term;
    }

    /* Add the +1 for the first column (j == 1) */
    if (j == 1)
        sum += 1;

    return sum;
}
