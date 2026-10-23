/*
 * A177677  The maximum integer dimension in which the volume of the
 *          hypersphere of radius n remains larger than 1.
 * https://oeis.org/A177677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max {d: Pi^d/2 * n^d / Gamma(1+d/2) > 1}
 * Range:     u8 n <= 3, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the maximum integer dimension d for which the volume of a
   hypersphere of radius n is greater than 1. */

#include <math.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

static const double PI_CONST = 3.1415926535897932384626433832795028841971;

/* Helper: true if volume of d‑dimensional sphere of radius n exceeds 1. */
static bool vol_gt1(idx_t n, idx_t d)
{
    /* n and d are assumed > 0. */
    double d_half = d * 0.5;
    double nd = (double)n;
    double logV = d_half * log(PI_CONST) + d * log(nd) - lgamma(1.0 + d_half);
    return logV > 0.0;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined, but return 0 safely */

    /* Set an initial upper bound that is guaranteed to exceed the answer.
       Empirically a(n) < 20*n*n for the range of interest (n <= 10000). */
    idx_t low = 0;
    idx_t high = 20ULL * n * n;
    if (high < 1)
        high = 1;   /* protect against overflow when n is huge */

    /* Ensure the high bound actually fails the inequality. */
    while (vol_gt1(n, high))
        high <<= 1;   /* double until it fails (worst case few iterations) */

    /* Binary search for the largest d with vol_gt1(n, d) == true. */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (vol_gt1(n, mid))
            low = mid;
        else
            high = mid;
    }

    /* low now contains the maximum dimension satisfying the inequality. */
    return (seq_t)low;
}
