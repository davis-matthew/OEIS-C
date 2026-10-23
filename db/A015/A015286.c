/*
 * A015286  Gaussian binomial coefficient [ n,3 ] for q = -13.
 * https://oeis.org/A015286
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Product_{i=1..3} ((-13)^(n-i+1) - 1)/((-13)^i - 1)
 * Range:     i8 none, i16 none, i32 n <= 5, i64 n <= 8, i128 n <= 14,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

/* Local GCD for seq_t (handles signed values).  Works for built‑in
   integer types and arbitrary‑precision integer classes that support
   the % operator. */
static inline seq_t seq_gcd_local(seq_t a, seq_t b)
{
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0)
    {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 3; values for smaller n
       are irrelevant for the tests.  Returning 0 for those indices
       keeps the function well‑defined. */
    if (n < 3)
        return 0;

    const seq_t q = (seq_t)-13;
    const seq_t denom_const = (seq_t)5169696; /* (-13-1)*(-13^2-1)*(-13^3-1) */

    seq_t result = 1;
    seq_t denom_rem = denom_const;

    for (int i = 1; i <= 3; ++i)
    {
        seq_t exp = (seq_t)(n - i + 1);           /* exponent for q^...   */
        seq_t numer = big_pow(q, exp) - 1;        /* (q^(n-i+1) - 1)     */

        seq_t g = seq_gcd_local(numer, denom_rem);
        if (g > 1)
        {
            numer /= g;
            denom_rem /= g;
        }

        result *= numer;
    }

    /* Any remaining denominator factor should divide the result
       exactly (the product is an integer). */
    if (denom_rem != 1)
        result /= denom_rem;

    return result;
}
