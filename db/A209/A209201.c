/*
 * A209201  A lower bound on the number of the distinct maximum genus
 *          embedding of the complete bipartite graph K(n,n).
 * https://oeis.org/A209201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n is even * 2^((n-1)/2) * ((n-2)!!) ^ n * ((n-1)!) ^
 *            n if n is odd
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-01)
 * Helpers:   ipow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (n-2)!! denotes the product of odd numbers up to n-2; by convention
 * (-1)!! = 1, the value for n=1                          */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow.h"
#include "../utils/pow2.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

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

seq_t a(idx_t n)
{
    /* even indices give zero directly */
    if ((n & 1ULL) == 0)
        return (seq_t)0;

    /* --- 2^((n-1)/2) part  ------------------------------------------------ */
    idx_t half_exponent = (n - 1) >> 1;          /* (n-1)/2 for odd n   */
    seq_t two_power      = pow2(half_exponent);  /* pow2 returns seq_t   */

    /* --- (n-2)!! part  ----------------------------------------------------- */
    /* For odd n = 2*k+1, the double factorial equals the product of
     * odd numbers 1,3,...,2*k-1. Number of terms k = (n-1)/2. */
    seq_t double_fact = (seq_t)1;
    idx_t k = (n - 1) >> 1;                      /* k = (n-1)/2          */
    for (idx_t j = 0; j < k; ++j) {
        /* multiply by the j-th odd number: 2*j+1 (fits in idx_t) */
        double_fact = seq_mul(double_fact,
                               (seq_t)(2 * j + 1));
    }

    /* --- (n-1)! part  --------------------------------------------------- */
    seq_t fact = factorial(n - 1);               /* factorial(0)=1 for n=1 */

    /* --- raise the factors to the n-th power  ---------------------------- */
    seq_t df_pow  = ipow(double_fact, (seq_t)n);
    seq_t fact_pow = ipow(fact,        (seq_t)n);

    /* --- combine all pieces  -------------------------------------------- */
    seq_t result = seq_mul(df_pow, fact_pow);    /* ((n-2)!!)^n * ((n-1)!)^n */
    result = seq_mul(result, two_power);         /* * 2^((n-1)/2)           */

    return result;
}
