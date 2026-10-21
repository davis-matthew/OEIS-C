/*
 * A227572  A sequence related to Le Corbusier's Modulor: round(phi^(-13 +
 *          n)*175).
 * https://oeis.org/A227572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(phi^(n-13)*175) where phi = (1+sqrt(5))/2
 * Range:     u8 n <= 12, u16 n <= 13, u32 n <= 23, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the exact algebraic form
   a + b*phi with integer coefficients a, b.
   The values are then converted to an integer by
   Stirling’s method: round((175*(2*a+b) + 175*b*sqrt(5))/2).
   The rounding is performed without floating point by
   using the integer square‑root function first_floor_sqrt
   from the utility library. */

#include "../utils/floor_sqrt.h"

/* Precomputed terms for n = 0 … 12 (offset 0) */
static const seq_t PREF[13] = {
    0ULL, 1ULL, 1ULL, 1ULL, 2ULL, 4ULL, 6ULL,
    10ULL, 16ULL, 26ULL, 41ULL, 67ULL, 108ULL
};

seq_t a(idx_t n)
{
    /* values that are already known separately */
    if (n < 13)
        return PREF[n];

    idx_t m = n - 13;                 /* exponent of phi */
    /* start with phi^0 = 1 = 1 + 0*phi   (a = 1, b = 0)      */
    seq_t a_coeff = 1;
    seq_t b_coeff = 0;

    /* iterate (a, b) → (b, a + b) until exponent m is reached */
    for (idx_t i = 0; i < m; ++i) {
        seq_t new_a = b_coeff;
        seq_t new_b = a_coeff + b_coeff;
        a_coeff = new_a;
        b_coeff = new_b;
    }

    /* S = 175 * b,   V = 175 * (2*a + b)   */
    seq_t S = 175 * b_coeff;
    seq_t V = 175 * (a_coeff + a_coeff + b_coeff);   /* 2*a + b */

    /* Compute integer part of the rounded value:
       a(n) = floor( ( V + S*sqrt(5) + 1 ) / 2 )  */
    seq_t C = V + 1;

    /* floor_sqrt(5 * S^2) gives floor(S*sqrt(5)) */
    seq_t S_sq = S * S;
    seq_t S_sq5 = S_sq * 5;
    seq_t LSqrt = floor_sqrt(S_sq5);

    seq_t result = (C + LSqrt) / 2;
    return result;
}
