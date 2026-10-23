/*
 * A022222  Gaussian binomial coefficients [ n,4 ] for q = 6.
 * https://oeis.org/A022222
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{i=1..4} (6^(n-i+1)-1)/(6^i-1) (Gaussian binomial [n,4]
 *            for q=6)
 * Range:     u8 none, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 4. For smaller n return 0. */
    if (n < 4)
        return 0;

    /* Raw denominators 6^i - 1 for i = 1..4, stored in a larger type to avoid
       truncation when seq_t is small. They are cast to seq_t when used. */
    static const unsigned long long denom_raw[4] = {5ULL, 35ULL, 215ULL, 1295ULL};

    seq_t result = 1;

    for (idx_t i = 1; i <= 4; ++i) {
        idx_t exp = n - i + 1;                /* n-i+1 */
        seq_t num = big_pow((seq_t)6, exp) - 1;
        seq_t den = (seq_t)denom_raw[i - 1];

        /* Reduce numerator and denominator together. */
        seq_t g1 = gcd(num, den);
        if (g1 > 1) {
            num /= g1;
            den /= g1;
        }

        /* Reduce current result with the remaining denominator. */
        seq_t g2 = gcd(result, den);
        if (g2 > 1) {
            result /= g2;
            den /= g2;
        }

        /* After the above reductions the remaining denominator should be 1. */
        result *= num;
    }

    return result;
}
