/*
 * A082070  Smallest prime that divides phi(n) and sigma(n) = A000203(n), or 1
 *          if phi(n) and sigma(n) are relatively prime.
 * https://oeis.org/A082070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime that divides φ(n) and σ(n), or 1 if φ(n)
 *            and σ(n) are coprime * (where φ = Euler's totient, σ = sum of
 *            divisors)
 * Range:     u8 n <= 1042, u16 n <= 9203, u32 n <= 9203, u64 n <= 9203,
 *            u128 n <= 9203, big any n
 * Helpers:   floor_sqrt, is_prime_small, phi, seq_gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/phi.h"
#include "../utils/sigma_of.h"
#include "../utils/seq_gcd.h"
#include "../utils/floor_sqrt.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* Compute phi(n) and sigma(n) */
    seq_t phi_n   = phi(n);
    seq_t sigma_n = sigma_of(n);

    /* GCD of phi(n) and sigma(n) */
    seq_t g = seq_gcd(phi_n, sigma_n);
    if (g == 1)
        return 1;                           /* coprime case */

    /* Find smallest prime divisor of g */
    seq_t sqrt_g = floor_sqrt(g);
    idx_t limit;
    if (sqrt_g <= (seq_t)UINT64_MAX)
        limit = (idx_t)sqrt_g;
    else
        limit = (idx_t)UINT64_MAX;          /* safety for very large g */

    for (idx_t p = 2; p <= limit; ++p) {
        if (!is_prime_small(p))
            continue;
        if (g % (seq_t)p == 0)
            return (seq_t)p;                /* smallest prime factor found */
    }

    /* If no small prime divisor was found, g itself is prime */
    return g;
}
