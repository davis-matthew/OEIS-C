/*
 * A058266  An approximation to sigma_{1/2}(n): floor( sum_{ d divides n }
 *          sqrt(d) ).
 * https://oeis.org/A058266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( sum_{d|n} sqrt(d) )
 * Range:     u8 n <= 1079, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the integer part of the sum of square roots of all divisors of n. */

#include <math.h>

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 */
    double sum = 0.0;
    idx_t i, j;

    /* Iterate over divisors up to sqrt(n) */
    for (i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            /* divisor i */
            sum += sqrt((double)i);

            /* paired divisor j = n / i, if distinct */
            j = n / i;
            if (j != i)
                sum += sqrt((double)j);
        }
    }

    /* Add a tiny epsilon to guard against floating‑point rounding
       errors that could push the truncated value one less. */
    const double EPS = 1e-12;
    unsigned long long val = (unsigned long long)(sum + EPS);
    return val;   /* implicit conversion to seq_t */
}
