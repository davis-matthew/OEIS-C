/*
 * A128936  a(n) = binomial(n, sum_digits_n).
 * https://oeis.org/A128936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = binomial(n, sum_digits(n))
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 46, u64 n <= 98, u128 n <= 588,
 *            big any n
 * Helpers:   digit_sum, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"
#include "../utils/gcd.h"

static seq_t binomial_multiplicative(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;          /* use symmetry */
    seq_t result = 1;       /* seq_t can be constructed from integer literal */

    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t numer = (seq_t)(n - k + i);   /* term in the numerator */
        seq_t denom = (seq_t)i;             /* term in the denominator */

        /* reduce numerator and denominator by their gcd */
        seq_t g = gcd(numer, denom);
        numer /= g;
        denom /= g;

        /* reduce result and remaining denominator */
        g = gcd(result, denom);
        result /= g;
        denom /= g;   /* denom should now be 1 */

        /* multiply the reduced numerator */
        result *= numer;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t num   = (seq_t)n;          /* cast index to seq_t for digit sum    */
    seq_t sum   = digit_sum(num);    /* sum of decimal digits of n            */
    idx_t  k    = (idx_t)sum;        /* sum is small, fits in idx_t           */
    return binomial_multiplicative(n, k);
}
