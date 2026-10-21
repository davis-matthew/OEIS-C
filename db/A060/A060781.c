/*
 * A060781  a(n) = lcm(sigma(n+1), sigma(n)), where sigma = A000203.
 * https://oeis.org/A060781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(σ(n+1), σ(n)) where σ(k) is the sum of divisors of k
 * Range:     u8 n <= 14, u16 n <= 142, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Compute the sum of divisors σ(k) for k ≥ 1.   */
static seq_t sigma(idx_t k)
{
    seq_t result = 1;
    idx_t m = k;

    for (idx_t p = 2; (seq_t)p * (seq_t)p <= m; ++p) {
        if (m % p == 0) {
            seq_t term = 1;          /* term = 1 + p + p² + … + pᵉ  */
            seq_t p_pow = 1;
            while (m % p == 0) {
                m /= p;
                p_pow *= p;
                term += p_pow;
            }
            result *= term;
        }
    }
    if (m > 1) {                     /* remaining prime factor */
        result *= ((seq_t)1 + (seq_t)m);
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sigma_n   = sigma(n);
    seq_t sigma_np  = sigma(n + 1);
    seq_t g         = gcd(sigma_n, sigma_np);
    return (sigma_n / g) * sigma_np;
}
