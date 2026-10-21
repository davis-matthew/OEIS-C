/*
 * A212406  Number of binary arrays of length 2*n+4 with no more than n ones
 *          in any length 2n subsequence (=50% duty cycle).
 * https://oeis.org/A212406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2*n+3) - 2*(59*n^2 - 84*n - 6) * binom(2*n-5, n-3) /*
 *            / (n*(n-1)) for n >= 3 Initial values (n = 1, 2): a(1) = 21,
 *            a(2) = 97 The binomial coefficient is computed multiplicatively
 *            with intermediate reductions to keep every intermediate value
 *            within the domain of seq_t
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"   /* seq_t gcd(seq_t a, seq_t b) */
#include "../utils/pow2.h"  /* seq_t pow2(idx_t exp) */

/* Compute binomial coefficient C(n, k) for 0 <= k <= n using GCD
   reductions to avoid overflow.  The result is of type seq_t. */
static seq_t binom_uint(idx_t n, idx_t k)
{
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = (seq_t)(n - k + i); /* numerator term          */
        seq_t den = (seq_t)i;           /* denominator term        */

        /* reduce res with den                     */
        seq_t g = gcd(res, den);
        res /= g;
        den /= g;

        /* reduce num with remaining den           */
        g = gcd(num, den);
        num /= g;
        den /= g;     /* den should be 1 now */

        res *= num;   /* exact multiplication; no overflow by design */
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)21;
    if (n == 2)
        return (seq_t)97;

    /* Power-of-two term 2^(2n+3) */
    seq_t pow_term = pow2((idx_t)(2 * n + 3));

    /* Binomial coefficient C(2n-5, n-3) */
    seq_t binom = binom_uint(2 * n - 5, n - 3);

    /* Coefficient 2*(59*n^2 - 84*n - 6) */
    seq_t n_seq = (seq_t)n;
    seq_t coef = (seq_t)59 * n_seq * n_seq;
    coef -= (seq_t)84 * n_seq;
    coef -= (seq_t)6;
    coef *= (seq_t)2;

    /* Denominator n*(n-1) */
    seq_t denom = n_seq * (n_seq - (seq_t)1);

    /* Reduce as much as possible before multiplying to avoid overflow */
    seq_t g1 = gcd(binom, denom);
    binom /= g1;
    denom /= g1;

    seq_t g2 = gcd(coef, denom);
    coef /= g2;
    denom /= g2;

    /* Now compute term = binom * coef / denom */
    seq_t temp = binom * coef; /* intermediate fits in seq_t after reduction */
    seq_t term = temp / denom;

    /* Final result a(n) = pow_term - term */
    return pow_term - term;
}
