/*
 * A031359  Bisection of A001615.
 * https://oeis.org/A031359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (2*n-1) * Π_{p|2*n-1} (1 + 1/p) (product over distinct
 *            odd primes)
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute m = 2*n - 1 (always odd) */
    seq_t m = (seq_t)2 * (seq_t)n - (seq_t)1;
    seq_t result = m;
    seq_t t = m;                 /* work copy for factorisation */

    /* trial division by odd numbers up to sqrt(t) */
    for (uint64_t p = 3; (seq_t)p * (seq_t)p <= t; p += 2)
    {
        if (t % (seq_t)p == (seq_t)0)
        {
            /* remove all powers of p from t */
            while (t % (seq_t)p == (seq_t)0)
                t /= (seq_t)p;
            /* incorporate the factor (p+1)/p once for each distinct prime p */
            result = result / (seq_t)p * ((seq_t)p + (seq_t)1);
        }
    }

    /* if a prime factor larger than sqrt(m) remains, process it */
    if (t > (seq_t)1)
        result = result / t * (t + (seq_t)1);

    return result;
}
