/*
 * A048786  Triangle of coefficients of certain exponential convolution
 *          polynomials.
 * https://oeis.org/A048786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the row‑by‑row reading of the triangle * a(r,m) = r! *
 *            4^(r‑m) * binomial(r‑1, m‑1) / m! (1 ≤ m ≤ r) * The index n is
 *            mapped to the unique pair (r,m) with * 1 + 2 + … + (r‑1) < n ≤ 1
 *            + 2 + … + r . * Then a(n) = a(r,m)
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-01)
 * Helpers:   binomial, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/ipow_idx.h"

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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* OEIS offset is 1; 0 is undefined */
        return 0;

    /* Find the row r such that 1+2+…+r ≥ n */
    idx_t r   = 0;
    idx_t cum = 0;
    while (cum < n) {
        ++r;
        cum += r;
    }

    /* Compute column m (1‑based) within that row */
    idx_t prev_sum = cum - r;   /* 1+2+…+(r-1) */
    idx_t m = n - prev_sum;

    /* Compute a(r,m) = r! * 4^(r‑m) * C(r‑1, m‑1) / m! */
    seq_t result = factorial(r);

    /* 4^(r-m) */
    result = seq_mul(result, ipow_idx(4, r - m));

    /* binomial(r-1, m-1) */
    result = seq_mul(result, binomial(r - 1, m - 1));

    /* divide by m! */
    result /= factorial(m);

    return result;
}
