/*
 * A211779  a(n) = Sum_{d_<n | n} sigma(d_<n), where d_<n = divisors of n that
 *          are less than n, sigma(x) = A000203(x).
 * https://oeis.org/A211779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d<n} sigma(d)
 * Range:     u8 n <= 71, u16 n <= 7559, u32 n <= 27144, u64 n <= 27144,
 *            u128 n <= 27144, big any n
 * Validated: standard, pass, 85 terms through n = 7915 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute sigma(p^exp) = 1 + p + p^2 + ... + p^exp */
static seq_t sigma_power(uint64_t p, idx_t exp)
{
    seq_t sum = 1;
    seq_t term = 1;
    for (idx_t i = 1; i <= exp; ++i) {
        term *= (seq_t)p;
        sum += term;
    }
    return sum;
}

/* Compute Sum_{i=0..exp} sigma(p^i) */
static seq_t sum_sigma_power(uint64_t p, idx_t exp)
{
    seq_t term = 1;          /* p^0 */
    seq_t sigma = 1;         /* sigma(p^0) */
    seq_t sum = 1;           /* sum_{i=0..0} sigma(p^i) */
    for (idx_t i = 1; i <= exp; ++i) {
        term *= (seq_t)p;         /* p^i */
        sigma += term;            /* sigma(p^i) */
        sum += sigma;             /* add to cumulative sum */
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;      /* n=1 has no proper divisors */

    uint64_t tmp = (uint64_t)n;
    seq_t prod_sigma = 1;            /* product of sigma(p^e)      */
    seq_t prod_sum   = 1;            /* product of sum_sigma(p^e) */

    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            idx_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            prod_sigma *= sigma_power(p, exp);
            prod_sum   *= sum_sigma_power(p, exp);
        }
    }

    /* Remaining prime factor, if any */
    if (tmp > 1) {
        /* exp = 1 for the remaining prime */
        prod_sigma *= (seq_t)(1 + tmp);          /* sigma(tmp^1) */
        prod_sum   *= (seq_t)(2 + tmp);          /* 1 + (1+tmp) = 2+tmp */
    }

    return prod_sum - prod_sigma;
}
