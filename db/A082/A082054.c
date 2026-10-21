/*
 * A082054  Sum of common prime divisors (without multiplicity) of sigma(n)
 *          and phi(n).
 * https://oeis.org/A082054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of common prime divisors (without multiplicity) of
 *            σ(n) and φ(n)
 * Range:     u8 n <= 1124, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factorize, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/phi.h"
#include "../utils/factorize.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;

    seq_t phi_val   = phi(n);     /* φ(n) */
    seq_t sigma_val = sigma_of(n); /* σ(n) */

    /* Convert to uint64_t – the values stay within 64‑bit for all n used. */
    uint64_t phi_u = (uint64_t)phi_val;
    uint64_t sigma_u = (uint64_t)sigma_val;

    /* Factor φ(n) and σ(n). */
    uint64_t primes_phi[64], expo_phi[64];
    uint64_t primes_sigma[64], expo_sigma[64];
    size_t cnt_phi = factorize(phi_u, primes_phi, (int *)expo_phi);
    size_t cnt_sigma = factorize(sigma_u, primes_sigma, (int *)expo_sigma);

    /* Sum common primes. */
    seq_t sum = 0;
    for (size_t i = 0; i < cnt_phi; ++i) {
        uint64_t p = primes_phi[i];
        for (size_t j = 0; j < cnt_sigma; ++j) {
            if (p == primes_sigma[j]) {
                sum += p;
                break;
            }
        }
    }
    return sum;
}
