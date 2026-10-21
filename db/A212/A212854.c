/*
 * A212854  Number of n X 7 arrays with rows being permutations of 0..6 and no
 *          column j greater than column j-1 in all rows.
 * https://oeis.org/A212854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 - 2*7^n - 2*21^n - 2*35^n + 3*42^n + 6*105^n + 3*140^n
 *            - 210^n - 12*420^n - 4*630^n + 5*840^n +10*1260^n - 6*2520^n +
 *            1*5040^n (offset 1, so a(1) is the first term above)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For other values we return 0. */
    if (n < 1)
        return 0;

    /* Cast the index to seq_t for use as an exponent. */
    seq_t exp = (seq_t)n;

    /* Bases and their corresponding signed integer coefficients. */
    static const int bases[] =
    {
        7,   21,  35,   42, 105, 140, 210, 420, 630, 840, 1260, 2520, 5040
    };
    static const int coeffs[] =
    {
        -2,  -2,  -2,   3,   6,   3,   -1,  -12,  -4,   5,  10,   -6,   1
    };
    const size_t count = sizeof(bases) / sizeof(bases[0]);

    /* Start with the constant term 1. */
    seq_t result = 1;

    for (size_t i = 0; i < count; ++i)
    {
        /* Compute base^n. */
        seq_t pow_val = big_pow((seq_t)bases[i], exp);

        /* Apply the signed coefficient.  Avoid overflow by building
           the signed product in two steps: multiply by the absolute
           value, then add or subtract depending on the sign. */
        seq_t abs_coeff = (coeffs[i] < 0 ? (seq_t)(-coeffs[i]) : (seq_t)coeffs[i]);

        pow_val *= abs_coeff;

        if (coeffs[i] > 0)
            result += pow_val;
        else
            result -= pow_val;
    }

    return result;
}
