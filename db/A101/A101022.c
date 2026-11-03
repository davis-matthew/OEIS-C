/*
 * A101022  Table of numerators of coefficients of certain rational
 *          polynomials.
 * https://oeis.org/A101022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 34, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Validated: partial, pass, 84 terms through n = 84 (2026-11-01)
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A101022 – numerators of coefficients of rational polynomials
    a(n) = numerator of the coefficient R(r,m) where
           r = smallest integer with r*(r+1)/2 ≥ n
           m = n – (r‑1)*r/2 – 1   (0‑based column in row r)
    R(r,m) = binom(r‑1,m) * 2^m
            ------------------------------------
            (m+1)*(2*m+1)*binom(2*m,m)
    The returned value is the reduced numerator.  */

#include <sys/types.h>   /* for idx_t and seq_t types used by the build */
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

/* Locate the row r (1‑based) such that r*(r+1)/2 ≥ n */
static idx_t find_row(idx_t n)
{
    idx_t low = 1, high = n, ans = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 cum = (unsigned __int128)mid * (mid + 1) / 2;
        if (cum >= n) {
            ans = mid;
            if (mid == 0) break;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return ans;
}

/* Euclidean algorithm for types that support % */

/* Integer power 2^exp for small exponents (exp ≤ 200) */
static seq_t pow2(idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= (seq_t)2;
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n = 0 – keep safe */

    /* Determine the row r and the 0‑based column m */
    idx_t r = find_row(n);
    idx_t prev = (r - 1) * r / 2;
    idx_t m = n - prev - 1;       /* 0 ≤ m < r */

    /* Compute numerator and denominator of R(r,m) */
    seq_t num = binomial(r - 1, m) * pow2(m);
    seq_t den = (seq_t)(m + 1);
    den *= (seq_t)(2 * m + 1);
    den *= binomial(2 * m, m);

    /* Reduce fraction and return numerator */
    seq_t g = gcd(num, den);
    return num / g;
}
