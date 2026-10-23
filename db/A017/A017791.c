/*
 * A017791  Binomial coefficients C(75,n).
 * https://oeis.org/A017791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(75,n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 23, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, if needed */

seq_t a(idx_t n)
{
    /* For n outside [0,75] the binomial coefficient is zero. */
    if (n > 75u)
        return (seq_t)0;

    /* For proving correctness we compute the prime exponents of
       C(75,n) by Legendre's formula.  The set of primes up to 75
       is known and fixed. */
    const idx_t primes[] = {
        2u, 3u, 5u, 7u, 11u, 13u, 17u, 19u, 23u, 29u,
        31u, 37u, 41u, 43u, 47u, 53u, 59u, 61u, 67u, 71u, 73u
    };
    const size_t prime_cnt = sizeof(primes) / sizeof(primes[0]);

    /* Use symmetry to keep k as the smaller of n and 75-n; this does
       not change the exponent calculation but keeps the intermediate
       values smaller. */
    idx_t k = n;
    if (k > 75u - k)
        k = 75u - k;

    seq_t result = (seq_t)1;

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p   = primes[pi];
        idx_t exp = 0;
        idx_t power = p;
        while (power <= 75u) {
            exp += (75u    / power) - (k       / power)
                 - ((75u - k) / power);
            if (power > 75u / p)
                break;
            power *= p;
        }
        for (idx_t i = 0; i < exp; ++i)
            result *= (seq_t)p;
    }

    return result;
}
