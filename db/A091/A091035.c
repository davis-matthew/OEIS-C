/*
 * A091035  Fifth column (k=6) of array A090438 ((4,2)-Stirling2).
 * https://oeis.org/A091035
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n-2,4) * (2*n)! / 6! (n >= 3)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3u) {
        /* The sequence is defined for n >= 3.  For lower indices we return 0. */
        return (seq_t)0;
    }

    /* Denominator 17280 = 2^7 * 3^3 * 5.  We will cancel these factors
       progressively to keep the intermediate result small. */
    const int denom_factors[3] = {5, 3, 2};
    const int denom_exps[3]   = {1, 3, 7};
    int rem[3];
    for (int i = 0; i < 3; ++i)
        rem[i] = denom_exps[i];

    idx_t two_n = n * 2u;         /* 2*n fits in idx_t for typical n (<=10000) */
    seq_t result = (seq_t)1;

    /* Multiply (2*n)! and cancel denominator factors when possible. */
    for (idx_t i = 1; i <= two_n; ++i) {
        result *= (seq_t)i;
        for (int j = 0; j < 3; ++j) {
            int f = denom_factors[j];
            while (rem[j] > 0 && !(result % (seq_t)f)) {
                result /= (seq_t)f;
                rem[j]--;
            }
        }
    }

    /* Multiply the extra factors (2*n-2), (2*n-3), (2*n-4), (2*n-5). */
    for (int e = 0; e < 4; ++e) {
        seq_t term = (seq_t)(two_n - 2u - (idx_t)e);
        result *= term;
        for (int j = 0; j < 3; ++j) {
            int f = denom_factors[j];
            while (rem[j] > 0 && !(result % (seq_t)f)) {
                result /= (seq_t)f;
                rem[j]--;
            }
        }
    }

    /* Any remaining denominator factors should now be canceled. */
    for (int j = 0; j < 3; ++j) {
        int f = denom_factors[j];
        while (rem[j] > 0) {
            /* The result should be divisible.  If not, the input was
               invalid or an overflow accident occurred. */
            result /= (seq_t)f;
            rem[j]--;
        }
    }

    return result;
}
