/*
 * A136125  Triangle read by rows: T(n,k) is the number of permutations of
 *          {1,2,...,n} in which the size of the last cycle is k (the cycles
 *          are ordered by increasing smallest elements; 1 <= k <=n).
 * https://oeis.org/A136125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 91, u64 n <= 210,
 *            u128 n <= 595, big any n
 * Validated: standard, pass, 61 terms through n = 7672 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A136125 – Triangle read by rows: a(n) = T(r,c)
 *  where r is the unique integer with r(r+1)/2 ≥ n and
 *  c = n – r(r‑1)/2   (1 ≤ c ≤ r).
 *
 *  T(r,c) = (r-1)!                 if c == r
 *          = r! / (c(c+1))         if c < r
 *
 *  This implements the OEIS sequence A136125 as defined in the
 *  reference – the sequence obtained by writing the triangle
 *  T(n,k) row by row.
 */

#include <stdint.h>

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
    /* Find the row r that contains the nth entry */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) {
        ++r;
    }

    /* column index within the row */
    idx_t c = n - r * (r - 1) / 2;   /* 1 ≤ c ≤ r */

    /* compute the value */
    if (c == r) {
        /* last element of the row */
        return factorial(r - 1);
    } else {
        seq_t numerator = factorial(r);
        seq_t denom     = (seq_t)c * (seq_t)(c + 1);
        return numerator / denom;
    }
}
