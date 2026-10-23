/*
 * A144733  Triangle read by rows, 2*A054533 - A054521.
 * https://oeis.org/A144733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = T(row,column) where T(n,k) = -I(gcd(n,k)=1) + 2 *
 *            Σ_{d|gcd(n,k)} d * μ(n/d)
 * Range:     i8 n <= 66, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   gcd_idx_t_func, mobius_calc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  T(n,k) is the triangle read by rows, 2*A054533 - A054521 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_idx_t_func.h"
#include "../utils/mobius_calc.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* sequence offset is 1; undefined for n < 1 */
    if (n == 0)
        return 0;

    /* --- Find the triangle row r such that r*(r+1)/2 >= n --- */
    idx_t low = 1, high = 1;
    while ( ( (__uint128_t)high * (high + 1) ) / 2 < n )
        high <<= 1;   /* exponential search for an upper bound */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ( ( (__uint128_t)mid * (mid + 1) ) / 2 >= n )
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* --- Column index k (1‑based) within the row --- */
    idx_t prev_total = ( (__uint128_t)(r - 1) * r ) / 2;
    idx_t k = n - prev_total;

    /* --- Compute gcd(r, k) --- */
    idx_t G = gcd_idx_t_func(r, k);

    /* --- Sum over divisors of G: d * μ(r/d) --- */
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= G; ++d) {
        if (G % d == 0) {
            idx_t d2 = G / d;

            /* first divisor d */
            idx_t r_over_d = r / d;
            int mu1 = mobius_calc(r_over_d);
            sum += (seq_t)d * mu1;

            /* second divisor d2, if distinct */
            if (d2 != d) {
                idx_t r_over_d2 = r / d2;
                int mu2 = mobius_calc(r_over_d2);
                sum += (seq_t)d2 * mu2;
            }
        }
    }

    /* --- Final value according to the definition --- */
    seq_t result = -(G == 1 ? 1 : 0) + (seq_t)2 * sum;
    return result;
}
