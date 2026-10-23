/*
 * A015385  Gaussian binomial coefficient [ n,9 ] for q=-13.
 * https://oeis.org/A015385
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Product_{i=1..9} ((-13)^(n-i+1)-1)/((-13)^i-1)
 * Range:     i8 n <= 9, i16 n <= 9, i32 n <= 9, i64 n <= 10, i128 n <= 12,
 *            big any n
 * Helpers:   gcd, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Gaussian binomial coefficient [n,9] for q = -13) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/pow_seq.h"

static bool denom_prepared = false;
static seq_t denom[9];

/* Precompute the fixed denominators ((-13)^i - 1) for i = 1..9 */
static void prepare_denominators(void)
{
    if (denom_prepared) return;
    seq_t base = (seq_t)-13;
    for (int i = 0; i < 9; ++i) {
        denom[i] = pow_seq(base, i + 1) - (seq_t)1;
    }
    denom_prepared = true;
}

/* Computes a(n) for n >= 9.  Behaviour for n < 9 is unspecified but
   we simply return 0 to avoid underflow. */
seq_t a(idx_t n)
{
    if (n < (idx_t)9) {
        return (seq_t)0;  /* offset behaviour not required */
    }

    prepare_denominators();

    seq_t result = (seq_t)1;
    seq_t base = (seq_t)-13;
    /* The numerators correspond to (-13)^(n-8), (-13)^(n-7), ..., (-13)^n */
    for (int i = 0; i < 9; ++i) {
        int exp = (int)(n - 8 + i);      /* exp ranges from n-8 to n   */
        seq_t numerator = pow_seq(base, exp) - (seq_t)1;
        seq_t d = denom[i];

        /* cancel gcd between numerator and denominator */
        seq_t g1 = gcd(numerator, d);
        numerator /= g1;
        d /= g1;

        /* cancel gcd between current result and remaining denominator */
        seq_t g2 = gcd(result, d);
        result /= g2;
        d /= g2;

        /* d should now be 1; multiply result by the remaining numerator */
        result *= numerator;
    }

    return result;
}
