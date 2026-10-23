/*
 * A183065  Triangle defined by g.f.: Sum_{n>=0} (4*n)!/n!^4 *
 *          x^(2*n)*y^n/(1-x-x*y)^(4*n+1), read by rows.
 * https://oeis.org/A183065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = coefficient of x^i y^j in Sum_{m>=0} (4m)!/m!^4 *
 *            x^(2m)*y^m/(1-x-x*y)^(4m+1), read by rows: a(n) = Triangle(r,k)
 *            where r = floor((sqrt(8n+1)-1)/2), k = n - r*(r+1)/2. Triangle
 *            entries T(r,k) are: T(r,k) = sum_{m=0}^{min(k, r/2)} (4m)!/m!^4
 *            * binom(r+2m, r-2m) * binom(r-2m, k-m). (binom(a,...
 * Range:     i8 n <= 10, i16 n <= 22, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binom.h"

/* Compute a(n) for OEIS sequence A183065. */
seq_t a(idx_t n)
{
    /* 1. Convert linear index n to row r and column k. */
    idx_t r = (idx_t)((sqrtl((long double)(8*n + 1)) - 1.0L)/2.0L);
    /* correct possible rounding errors */
    while ((r+1)*(r+2)/2 <= n) ++r;
    while (r*(r+1)/2 > n) --r;
    idx_t k = n - r*(r+1)/2;

    seq_t result = 0;

    /* 2. Sum over m from 0 to min(k, r/2). */
    idx_t max_m = r/2;
    if (max_m > k) max_m = k;

    for (idx_t m = 0; m <= max_m; ++m)
    {
        /* 2a. s = r - 2m, t = k - m.  (t <= s by construction) */
        idx_t s = r - 2*m;
        idx_t t = k - m;

        /* 2b. Compute (4m)!/m!^4 as product of three binomials. */
        seq_t term = binom(4*m, m);
        term *= binom(3*m, m);
        term *= binom(2*m, m);

        /* 2c. Multiply by the remaining binomials. */
        term *= binom(r + 2*m, s);
        term *= binom(s, t);

        /* 2d. Accumulate result. */
        result += term;
    }

    return result;
}
