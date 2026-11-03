/*
 * A152818  Array read by antidiagonals: A(n,k) = (k+1)^n*(n+k)!/n!.
 * https://oeis.org/A152818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (k+1)^(n-k) * k! * binomial(n+k, k) where * n,k are
 *            determined by the antidiagonal reading of the array * A(n,k) =
 *            (k+1)^n * (n+k)! / n! with offset 0. * Precompute the
 *            antidiagonal index: for a given linear index n * find s such
 *            that (s+1)(s+2)/2 > n >= s(s+1)/2; * let offset = n - s(s+1)...
 * Range:     u8 n <= 16, u16 n <= 31, u32 n <= 71, u64 n <= 180,
 *            u128 n <= 480, big any n
 * Validated: standard, pass, 61 terms through n = 1189 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"

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
    /* Find s such that count(s-1) <= n < count(s),
     * where count(s) = (s+1)(s+2)/2. */
    idx_t s = 0;
    while (((s + 1) * (s + 2)) / 2 <= n)
        ++s;

    /* count of terms before current antidiagonal */
    idx_t prev_count = (s * (s + 1)) / 2;

    /* offset within antidiagonal */
    idx_t k = n - prev_count;          /* column index */
    idx_t n_coord = s - k;             /* row index */

    /* Compute the term as (k+1)^(n_coord) * k! * binomial(n_coord+k, k) */
    seq_t val = ipow((seq_t)(k + 1), (seq_t)n_coord);          /* (k+1)^(n_coord) */
    val *= factorial(k);                                            /* * k! */
    val *= binomial(n_coord + k, k);                               /* * C(n+k, k) */

    return val;
}
