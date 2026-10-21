/*
 * A073816  a(n) = 2^sigma(n) mod n.
 * https://oeis.org/A073816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^sigma(n) mod n (sigma = sum of divisors)
 * Range:     u8 n <= 265, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_pow.h"

/* Compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    /* Handle n = 1 directly (sigma(1)=1, 2^1 mod 1 = 0) */
    if (n == 1) return 0;

    idx_t temp = n;
    seq_t sigma = 1; /* running product of sigma factors */

    /* Factorise n and accumulate sigma */
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            /* Determine exponent of this prime factor */
            seq_t factor_sum = 1;   /* start with p^0 = 1 */
            seq_t pow = 1;          /* current power of p (p^k) */

            while (temp % p == 0) {
                temp /= p;
                pow *= (seq_t)p;     /* pow = p^k */
                factor_sum += pow;   /* add next term of geometric series */
            }

            sigma *= factor_sum;     /* combine with overall sigma */
        }
    }

    /* If a prime factor > sqrt(original n) remains */
    if (temp > 1) {
        /* Remaining factor is a prime with exponent 1, sum = 1 + p */
        sigma *= ((seq_t)temp + 1);
    }

    /* Compute 2^sigma mod n using provided modular exponentiation */
    return mod_pow(2, (idx_t)sigma, n);
}
