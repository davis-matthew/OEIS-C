/*
 * A137388  Triangle t(n,m)= (m^2-1) * binomial(n,m) * (n+2)/(n+2-m) read by
 *          rows, 0<=m<=n.
 * https://oeis.org/A137388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign, easy
 * Formula:   a(n) = t(r,m) where * r = ⌊(√(8n+1)−1)/2⌋ * m = n − r(r+1)/2 *
 *            t(r,m) = (m²−1)·C(r,m)·(r+2)/(r+2−m) * * (C(r,m) is the binomial
 *            coefficient.) * * This computes the terms of OEIS A137388 by
 *            evaluating the triangle * entry corresponding to the flattened
 *            row‑by‑row index n
 * Range:     i8 n <= 12, i16 n <= 62, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

/* Integer square root for unsigned 64‑bit values: floor(sqrt(x)) */
static idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if (mid > 0 && mid > x / mid) {
            hi = mid - 1;
        } else {
            lo = mid;
        }
    }
    return lo;
}

/* Compute the n-th term of A137388 */
seq_t a(idx_t n)
{
    /* r = floor((sqrt(8n+1)-1)/2) */
    idx_t r = (isqrt_u64((idx_t)8 * n + 1) - 1) / 2;

    /* Adjust r in case the initial estimate is off by one */
    while (((idx_t)(r + 1) * (r + 2)) / 2 <= n) {
        r++;
    }
    while (((idx_t)r * (r + 1)) / 2 > n) {
        r--;
    }

    /* m is the position within row r  (0 ≤ m ≤ r) */
    idx_t m = n - ((idx_t)r * (r + 1)) / 2;

    /* Binomial coefficient C(r,m) */
    seq_t bin = binomial(r, m);

    /* Compute the formula components in seq_t */
    seq_t factor = ((seq_t)m * (seq_t)m) - 1;      /* m² – 1        */
    seq_t denom  = (seq_t)(r + 2 - m);             /* r+2 – m       */
    seq_t num    = (seq_t)(r + 2);                 /* r+2           */

    /* Reduce fractions using GCD to keep intermediate values small */
    seq_t g;
    g = gcd(factor, denom);
    factor /= g;
    denom  /= g;

    g = gcd(bin, denom);
    bin   /= g;
    denom /= g;

    /* Final assembly: (bin * factor * num) / denom */
    seq_t result = bin;
    result *= factor;
    result *= num;
    if (denom != 1) {
        result /= denom;
    }
    return result;
}
