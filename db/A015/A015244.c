/*
 * A015244  Inverse of 1235th cyclotomic polynomial.
 * https://oeis.org/A015244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of xⁿ in the expansion of 1/Φ₁₂₃₅(x)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   div_xpow_minus_one, mul_xpow_minus_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Inverse of the 1235‑th cyclotomic polynomial Φ₁₂₃₅(x).
   The sequence a(n) (n ≥ 0) consists of the coefficients of the power
   series 1/Φ₁₂₃₅(x); it is periodic with period 1235. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/div_xpow_minus_one.h"
#include "../utils/mul_xpow_minus_one.h"

/* Möbius function μ(m) for 0 < m ≤ 1235. */
static int mobius_uint64(uint64_t m)
{
    if (m == 1) return 1;
    int cnt = 0;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            m /= p;
            cnt++;
            if (m % p == 0) return 0;   /* squared factor */
        }
    }
    if (m > 1) cnt++;                    /* remaining prime factor */
    return (cnt & 1) ? -1 : 1;           /* (-1)^cnt */
}

/* Return the coefficient a(n) of the series 1/Φ₁₂₃₅(x). */
seq_t a(idx_t n)
{
    /* Cached values: the sequence is periodic with period 1235. */
    static bool init = false;
    static seq_t seq[1235];
    static seq_t coeff[2001];   /* maximum degree φ(1235) = 864 */
    static size_t deg;          /* current degree of coeff */

    if (!init) {
        /* Build Φ₁₂₃₅(x) = ∏_{d|1235} (xᵈ - 1)^{μ(1235/d)}. */
        const unsigned int divisors[8] = {1,5,13,19,65,95,247,1235};
        size_t muls[8], divs[8];
        size_t nmuls = 0, ndivs = 0;

        for (size_t i = 0; i < 8; ++i) {
            unsigned int d = divisors[i];
            unsigned int m = 1235 / d;
            int mu = mobius_uint64(m);
            if (mu == 1)      muls[nmuls++] = d;
            else if (mu == -1) divs[ndivs++] = d;
        }

        /* Start from the polynomial 1. */
        coeff[0] = 1;
        deg = 0;

        for (size_t i = 0; i < nmuls; ++i) {
            mul_xpow_minus_one(coeff, &deg, muls[i]);
        }
        for (size_t i = 0; i < ndivs; ++i) {
            div_xpow_minus_one(coeff, &deg, divs[i]);
        }

        /* Compute the sequence a(k) for 0 ≤ k < 1235. */
        seq[0] = 1;          /* a(0) = 1 */
        for (size_t k = 1; k < 1235; ++k) {
            seq_t sum = 0;
            size_t up = (k <= deg) ? k : deg;
            for (size_t j = 1; j <= up; ++j) {
                sum += coeff[j] * seq[k - j];
            }
            seq[k] = -sum;
        }
        init = true;
    }
    /* Return the periodic term. */
    return seq[(size_t)(n % 1235)];
}
