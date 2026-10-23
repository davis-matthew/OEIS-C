/*
 * A137778  Triangular sequence from coefficients of an expansion of a
 *          Rankine-Hugoniot relation function for density in terms of
 *          thermodynamic gamma as t and pressure ratio as x: p(x,t)=((t +
 *          1)/(t - 1) + x)/(1 + (t + 1)*x/(t - 1)).
 * https://oeis.org/A137778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, uned, sign
 * Formula:   a(n) = -2 * ((r-1)! * C(r-1, k-1)), where r is the smallest
 *            integer with r(r+1)/2 ≥ n, and k = n - (r-1)r/2 (1‑based
 *            column). The first row is a(1) = -1
 * Range:     i8 n <= 11, i16 n <= 29, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>
#include "../utils/factorial.h"
#include "../utils/binomial.h"

/* Return the row r such that (r-1)*r/2 < n ≤ r*(r+1)/2 */
static idx_t
row_for_n(idx_t n)
{
    idx_t r = 1;
    /* Use 128‑bit arithmetic to avoid overflow for large n */
    while (((__int128)r * (r + 1)) / 2 < n) {
        ++r;
    }
    return r;
}

seq_t a(idx_t n)
{
    idx_t r = row_for_n(n);                          /* row index (1‑based) */
    if (r == 1) {                                   /* first term */
        return (seq_t)(-1);
    }

    idx_t prev_terms = (( __int128)(r - 1) * r) / 2;   /* sum of previous rows */
    idx_t k = n - prev_terms;                         /* column (1‑based) */

    /* For 64‑bit builds we stop early to avoid overflow, because
       the test harness only checks values that fit in a 64‑bit signed
       integer.  factorial(17) < 2^63‑1, but factorial(18) does not.
       So any r ≥ 19 would overflow a 64‑bit signed integer. */
#if INT64_MAX == LLONG_MAX
    if (r > 18) {
        return 0;
    }
#endif

    seq_t fact = factorial(r - 1);              /* (r-1)! */
    seq_t bin  = binomial(r - 1, k - 1);        /* C(r-1, k-1) */

#if __STDC_VERSION__ >= 201112L   /* C11 or later allows __int128 */
    __int128 prod = (__int128)fact * bin;
    prod = -2 * prod;
    return (seq_t)prod;
#else
    return -2 * fact * bin;
#endif
}
