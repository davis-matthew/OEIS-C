/*
 * A176122  Triangle, read by rows, T(n,k) = Sum_{j=1..k} binomial(n-1,
 *          j-1)*binomial(k, j - 1)*(j-1)!.
 * https://oeis.org/A176122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where T(r,k) = Σ_{j=1..k} binomial(r-1, j-1) *
 *            binomial(k, j-1) * (j-1)! r = smallest integer with r(r+1)/2 ≥
 *            n, k = n - r(r-1)/2
 * Range:     u8 n <= 14, u16 n <= 34, u32 n <= 77, u64 n <= 208,
 *            u128 n <= 559, big any n
 * Validated: standard, pass, 71 terms through n = 5015 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function uses the helper functions 'binom' and 'factorial'
    defined in the libutils directory.  */
#include <stdint.h>
#include "../utils/binomial.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset starts at 1, but safeguard */

    /* Find the row r such that r*(r+1)/2 >= n. */
    idx_t r = 1;
    idx_t tri = 1;               /* cumulative triangular number */
    while (tri < n) {
        ++r;
        tri += r;
    }

    /* Compute column k within that row. */
    idx_t prev = tri - r;        /* sum up to row r-1 = r*(r-1)/2 */
    idx_t k = n - prev;          /* 1 .. r */

    /* Sum the defining expression for T(r,k). */
    seq_t sum = 0;
    for (idx_t j = 1; j <= k; ++j) {
        seq_t term = binomial(r - 1, j - 1);
        term *= binomial(k, j - 1);
        term *= factorial(j - 1);
        sum += term;
    }
    return sum;
}
