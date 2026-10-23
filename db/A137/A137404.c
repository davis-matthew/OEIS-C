/*
 * A137404  a(n) =
 *          ceiling(10*(prime(n+1)-prime(n))/(2*floor(sqrt(prime(n)*log(prime(n)))))).
 * https://oeis.org/A137404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) =
 *            ceil(10*(prime(n+1)-prime(n))/(2*floor(sqrt(prime(n)*log(prime(n))))))
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1  (a(1) is the first term listed) */

#include <math.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; access primes prime(n) and prime(n+1) */

    uint64_t p_n   = (uint64_t)get_prime((uint32_t)n);
    uint64_t p_n1  = (uint64_t)get_prime((uint32_t)(n + 1));

    /* Difference of consecutive primes */
    uint64_t diff = p_n1 - p_n;

    /* Numerator: 10 * difference */
    uint64_t num = 10 * diff;

    /* Compute floor(sqrt(p_n * log(p_n))) using double precision */
    double val = (double)p_n * log((double)p_n);
    double sqrt_val = sqrt(val);
    uint64_t floor_sqrt = (uint64_t)floor(sqrt_val);

    /* Denominator: 2 * floor_sqrt */
    uint64_t den = 2 * floor_sqrt;

    /* Ceil division: ceil(num / den) = (num + den - 1) / den */
    uint64_t result = (num + den - 1) / den;

    return (seq_t)result;
}
