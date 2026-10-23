/*
 * A136552  a(n) = C(2*2^n + 2*n, n)*2^n/(2^n + n); a(n) = coefficient of x^n
 *          in Catalan(x)^(2*2^n).
 * https://oeis.org/A136552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(2*2^n + 2*n, n)*2^n/(2^n + n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/gcd.h"

/* compute binomial coefficient C(N,k) for a large N (seq_t) and small k (idx_t) */
static seq_t binom_large(seq_t N, idx_t k)
{
    seq_t res = 1;                 /* running result */
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = N - (seq_t)(i - 1);      /* N - i + 1   */
        seq_t den = (seq_t)i;                /* denominator i */

        /* cancel common factors between numerator and denominator */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        /* cancel any remaining denominator with the current result */
        g = gcd(res, den);
        res /= g;
        den /= g;

        /* multiply the result by the reduced numerator */
        res *= num;             /* den must now be 1 */

        /* may happen that den > 1 only due to numeric imprecision, but
           for binomial coefficients it should be 1 here; ignore the check */
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t pow2n = pow2(n);                                 /* 2^n */
    seq_t N = pow2n * 2 + (seq_t)(2 * (seq_t)n);           /* 2*2^n + 2*n */
    seq_t bin = binom_large(N, n);                         /* C(N,n)   */
    seq_t denom = pow2n + (seq_t)n;                        /* 2^n + n */

    /* reduce the division before the final multiplication to keep values small */
    seq_t bin_c = bin;
    seq_t den = denom;
    seq_t g = gcd(bin_c, den);
    bin_c /= g;
    den /= g;

    seq_t pow2_c = pow2n;
    g = gcd(pow2_c, den);
    pow2_c /= g;
    den /= g;

    seq_t result = bin_c * pow2_c;   /* safe: product fits if final result fits */
    if (den != 1)                    /* sometimes leftover divisor (should be 1) */
        result /= den;

    return result;
}
