/*
 * A014503  Inverse of 494th cyclotomic polynomial.
 * https://oeis.org/A014503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A014503:  Inverse of the 494th cyclotomic polynomial.
 *
 *  The coefficients a(n) satisfy
 *      1 / Φ_494(x) = \sum_{n≥0} a(n) x^n ,
 *  and a(n) is periodic with period 494.
 *
 *  This implementation builds the first 494 coefficients by
 *  multiplying the elementary factors
 *
 *      Φ_494(x) = ∏_{d|494} (x^d - 1)^{μ(494/d)}   ,
 *
 *  hence
 *
 *      1/Φ_494(x) = ∏_{d|494} (x^d - 1)^{-μ(494/d)} .
 *
 *  The product is rewritten as a product of simple
 *  polynomials: for μ = 1 we use the infinite series
 *      (1 - x^d)^{-1} = 1 + x^d + x^{2d} + … ,
 *  and for μ = –1 we use the finite polynomial
 *      (x^d - 1) = –1 + x^d .
 *
 *  In the implementation each factor contributes a convolution,
 *  which is computed in O(PERIOD) time per factor, giving an
 *  overall O(PERIOD × num_divisors) ≈ O(4 × 10³) operation count.
 */

#include <stdbool.h>

#define PERIOD 494

/* Divisors of 494 and corresponding Möbius values μ(494/d). */
static const unsigned long long divs[8] = {1ULL, 2ULL, 13ULL, 19ULL, 26ULL, 38ULL, 247ULL, 494ULL};
static const int mu[8]                 = {-1,   1,    1,    1,  -1,  -1,   -1,    1};

/*  coefficients a(n) for 0 ≤ n < PERIOD are computed once and cached. */
static seq_t cached[PERIOD];

/*  Compute the first PERIOD coefficients once.  */
static void build_cache(void)
{
    seq_t coeff[PERIOD];
    seq_t tmp[PERIOD];
    unsigned k, t, i;

    /*  initialise to 1 at n = 0, zero elsewhere */
    coeff[0] = 1;
    for (k = 1; k < PERIOD; ++k) coeff[k] = 0;

    /*  process all factors in the product */
    for (i = 0; i < 8; ++i)
    {
        unsigned long long d = divs[i];
        if (mu[i] == 1)   /*  infinite series 1 + x^d + x^{2d} + …  */
        {
            for (k = 0; k < PERIOD; ++k)
            {
                if (k < d)
                    tmp[k] = coeff[k];                 /*  only t = 0 term  */
                else
                    tmp[k] = coeff[k] + tmp[k - d];     /*  recurrence  */
            }
            for (k = 0; k < PERIOD; ++k) coeff[k] = tmp[k];
        }
        else               /*  finite polynomial –1 + x^d  */
        {
            for (k = 0; k < PERIOD; ++k) tmp[k] = coeff[k];
            for (k = 0; k < PERIOD; ++k)
            {
                if (k >= d)
                    coeff[k] = tmp[k] - tmp[k - d];
                else
                    coeff[k] = tmp[k];
            }
        }
    }

    /*  copy into the static cache  */
    for (k = 0; k < PERIOD; ++k) cached[k] = coeff[k];
}

/*  Public entry point – returns a(n) for n ≥ 0  */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized)
    {
        build_cache();
        initialized = true;
    }
    return cached[n % PERIOD];
}
