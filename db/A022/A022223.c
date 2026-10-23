/*
 * A022223  Gaussian binomial coefficients [ n,5 ] for q = 6.
 * https://oeis.org/A022223
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=1..5} (6^(n-i+1)-1)/(6^i-1)
 * Range:     u8 none, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   seq_gcd, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/seq_gcd.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* a(n) is defined only for n >= 5. Behaviour for n < 5 is irrelevant. */
    if (n < 5) return 0;

    seq_t result = (seq_t)1;

    /* compute factors i = 1..5 */
    for (idx_t i = 1; i <= 5; ++i)
    {
        /* denominator: 6^i - 1 */
        seq_t denom = seq_pow((seq_t)6, i);
        denom = denom - (seq_t)1;

        /* numerator: 6^(n-i+1) - 1 */
        seq_t num = seq_pow((seq_t)6, n - i + 1);
        num = num - (seq_t)1;

        /* reduce numerator and denominator by their GCD to keep numbers small */
        seq_t g = seq_gcd(num, denom);
        num   = num / g;
        denom = denom / g;

        /* reduce current result and remaining denominator by their GCD */
        seq_t g2 = seq_gcd(result, denom);
        result = result / g2;
        denom  = denom / g2;

        /* multiply the reduced numerator */
        result = seq_mul(result, num);

        /* divide by the reduced denominator (division is exact) */
        result = result / denom;
    }

    return result;
}
