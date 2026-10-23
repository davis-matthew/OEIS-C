/*
 * A015323  Gaussian binomial coefficient [ n,6 ] for q = -2.
 * https://oeis.org/A015323
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Gaussian binomial coefficient [n,6] for q = -2
 * Range:     u8 none, u16 n <= 8, u32 n <= 11, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n must be >= 6; behaviour for smaller n is irrelevant */
    seq_t res = 1;
    for (idx_t i = 1; i <= 6; ++i)
    {
        idx_t exp_num = n + 1 - i;   /* exponent for numerator */
        idx_t exp_den = i;          /* exponent for denominator */

        seq_t num_pp = pow2(exp_num); /* 2^exp_num */
        seq_t den_pp = pow2(exp_den); /* 2^exp_den */

        seq_t num_abs, den_abs;

        /* abs((-2)^exp - 1) */
        if ((exp_num & 1) == 0)   /* even exponent -> positive 2^k - 1 */
            num_abs = num_pp - 1;
        else                      /* odd exponent  -> -(2^k + 1) */
            num_abs = num_pp + 1;

        /* abs((-2)^exp - 1) for denominator */
        if ((exp_den & 1) == 0)
            den_abs = den_pp - 1;
        else
            den_abs = den_pp + 1;

        /* Reduce the factor by its internal GCD */
        seq_t g = gcd(num_abs, den_abs);
        num_abs /= g;
        den_abs /= g;

        /* Cancel any remaining denominator with current result first */
        g = gcd(res, den_abs);
        res /= g;
        den_abs /= g;

        /* Multiply by numerator and finish the division */
        res *= num_abs;
        res /= den_abs; /* den_abs should now be 1 */
    }

    return res;
}
