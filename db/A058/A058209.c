/*
 * A058209  a(n) = floor( exp(gamma) n log log n ) - sigma(n), where gamma is
 *          Euler's constant (A001620) and sigma(n) is sum of divisors of n
 *          (A000203).
 * https://oeis.org/A058209
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, nice, easy
 * Formula:   a(n) = floor(exp(gamma) * n * log(log n)) - sigma(n) (n >= 2)
 * Range:     i8 n <= 82, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

static const long double EXP_GAMMA = 1.781072417990128172L;

/* Compute sigma(n) – the sum of positive divisors of n. */
static seq_t sigma_of(idx_t n)
{
    seq_t result = 1;
    unsigned long long m = n;
    for (unsigned long long p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            seq_t sum = 1;      /* sum of p^0 + p^1 + ... + p^k */
            seq_t term = 1;
            while (m % p == 0)
            {
                m   /= p;
                term *= (seq_t)p;
                sum  += term;
            }
            result *= sum;
        }
    }
    if (m > 1)                    /* remaining prime factor */
        result *= (seq_t)(m + 1);  /* (p^1 - 1)/(p - 1) = p + 1 */
    return result;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                /* undefined for n < 2; return 0 */

    /* Compute the integer part of exp(gamma) * n * log(log n) */
    long double ln_n    = logl((long double)n);     /* ln(n)       */
    long double ln_ln   = logl(ln_n);                /* ln(ln(n))   */
    long double prod    = EXP_GAMMA * (long double)n * ln_ln;
    long long  int_part = (long long)floorl(prod);   /* fits in 64‑bit for n <= 10000 */

    seq_t floor_val = (seq_t)int_part;

    /* Subtract sigma(n) */
    seq_t sigma = sigma_of(n);
    return floor_val - sigma;
}
