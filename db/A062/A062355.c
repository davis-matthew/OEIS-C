/*
 * A062355  a(n) = d(n) * phi(n), where d(n) is the number of divisors
 *          function.
 * https://oeis.org/A062355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult, changed
 * Formula:   a(n) = d(n) * φ(n) (number of divisors times Euler's totient)
 * Range:     u8 n <= 71, u16 n <= 5039, u32 n <= 9785, u64 n <= 9785,
 *            u128 n <= 9785, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return a(n) for n ≥ 1.  For n = 0 the value is undefined by the OEIS
   offset; this implementation returns 0.  The function does not keep
   any state, so calls may be in arbitrary order.  It uses only integer
   arithmetic, and for small n (≤ 10 000) the running time is well below
   2 s. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset not defined */

    uint64_t m    = (uint64_t)n;              /* original n */
    uint64_t tau  = 1;                        /* divisor count d(n) */
    uint64_t phi  = m;                        /* Euler's totient φ(n) */
    uint64_t tmp  = m;                        /* temporary value for factorisation */

    /* Trial division up to sqrt(tmp).  Using p <= tmp / p avoids overflow. */
    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            tau *= (exp + 1);                 /* multiply by (e+1) for this prime */
            phi -= phi / p;                   /* update φ:  φ = φ * (1 - 1/p) */
        }
    }

    /* If a prime factor larger than sqrt(n) remains. */
    if (tmp > 1) {
        tau *= 2;                             /* the remaining prime contributes (1+1) */
        phi -= phi / tmp;
    }

    /* Compute the product. Cast to seq_t before multiplication so
       the result is expressed in the correct type. */
    return (seq_t)tau * (seq_t)phi;
}
