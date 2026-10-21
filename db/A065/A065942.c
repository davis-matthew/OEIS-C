/*
 * A065942  Central column of triangle A065941.
 * https://oeis.org/A065942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n + floor(n/2), n) Computed via a multiplicative
 *            algorithm with cancellation of common factors using gcd
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 49, u128 n <= 95,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Computes binomial(n, k) exactly for any seq_t.
   The algorithm reduces the fraction step by step
   to avoid intermediate overflow. */
static seq_t
binom(idx_t n, idx_t k)
{
    if (k > n)
        return 0;                /* undefined, but safe fallback */

    /* Use the symmetry property to minimise steps. */
    if (k > n - k)
        k = n - k;

    seq_t result = 1;

    for (idx_t i = 1; i <= k; ++i)
    {
        /* (n - k + i) is the next numerator term. */
        seq_t num = (seq_t)(n - k + i);
        seq_t den = (seq_t)i;

        /* Cancel gcd(num, den) first. */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        /* Then cancel gcd(result, den). */
        g = gcd(result, den);
        result /= g;
        den /= g;

        /* Multiply the remaining numerator part. */
        result *= num;

        /* Any remaining denominator must divide the new result.
           It should be 1, but perform the division just in case. */
        if (den != 1)
            result /= den;
    }

    return result;
}

/* Public API required by the build system. */
seq_t
a(idx_t n)
{
    /* m = n + floor(n/2) */
    idx_t m = n + (n / 2);
    /* k = floor(n/2) is the smaller factor in the binomial. */
    idx_t k = n / 2;
    return binom(m, k);
}
