/*
 * A035226  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s) + Kronecker(m,p)*p^(-2s))^(-1) for m
 *          = 44.
 * https://oeis.org/A035226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(44,d) (Kronecker symbol)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_for_prime_exp, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/jacobi_symbol.h"
#include "../utils/factor_for_prime_exp.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safety: sequence starts at offset 1 */

    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    uint64_t exp;

    /* Factor out the prime 2 – Kronecker(44,2)=0 → factor 1 (do nothing) */
    exp = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        exp++;
    }

    /* Factor out the prime 11 – Kronecker(44,11)=0 → factor 1 */
    exp = 0;
    while (m % 11ULL == 0) {
        m /= 11ULL;
        exp++;
    }

    /* Factor remaining odd primes up to sqrt(m) */
    for (uint64_t p = 3; m / p >= p; p += 2) {
        if (m % p == 0) {
            exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            int K = jacobi_symbol(44, p);      /* p is prime → K = ±1 */
            result *= factor_for_prime_exp(exp, K);
        }
    }

    /* If a prime > sqrt(n) remains, it appears with exponent 1 */
    if (m > 1) {
        int K = jacobi_symbol(44, m);          /* m is prime (odd, ≠ 11) */
        result *= factor_for_prime_exp(1, K);
    }

    return result;
}
