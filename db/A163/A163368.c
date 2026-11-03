/*
 * A163368  a(n) = phi(sigma(tau(n))).
 * https://oeis.org/A163368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phi( sigma( tau(n) ) ) (OEIS A163368)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 101 terms through n = 896 (2026-11-01)
 * Helpers:   build_primes_upto, isqrt, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/isqrt.h"
#include "../utils/totient.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety – offset starts at 1 */

    if (n == 1) return 1;           /* tau(1)=1, sigma(1)=1, phi(1)=1 */

    /* factorise n via trial division using primes up to sqrt(n) */
    uint64_t nn = (uint64_t)n;
    idx_t lim = isqrt(nn);

    uint64_t *primes = NULL;
    size_t  pcnt   = 0;
    build_primes_upto(lim, &primes, &pcnt);

    uint64_t temp = nn;
    idx_t    tau  = 1;               /* product of (exp+1) */

    for (size_t i = 0; i < pcnt && primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        unsigned exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }
        if (exp > 0) {
            tau *= (exp + 1);
        }
    }
    if (temp > 1) {
        tau *= 2;                    /* leftover prime factor */
    }

    free(primes);

    seq_t sigma  = sigma_of(tau);      /* tau fits in idx_t */
    seq_t result = totient((idx_t)sigma);  /* final value */

    return result;
}
