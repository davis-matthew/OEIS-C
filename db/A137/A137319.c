/*
 * A137319  Start with the set of natural numbers. Add 1 to every 2nd term, 2
 *          to every 3rd term, 3 to every 4th term, etc.
 * https://oeis.org/A137319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n + sigma(n) - tau(n) (sigma = sum of divisors, tau =
 *            number of divisors)
 * Range:     u8 n <= 79, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt.h"   /* Provides _floor_isqrt_u64 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, but return 0 */

    seq_t sigma = 1;   /* product of sums of prime powers */
    seq_t tau   = 1;   /* product of (exponent+1) */
    idx_t temp = n;

    /* factor 2 separately */
    if (temp % 2 == 0) {
        idx_t e = 0;
        seq_t pk = 1;        /* p^i */
        seq_t sum = 1;       /* 1 + p + p^2 + ... + p^e */
        while (temp % 2 == 0) {
            ++e;
            temp /= 2;
            pk = pk * (seq_t)2;
            sum += pk;
        }
        sigma = sigma * sum;
        tau   = tau * (e + 1);
    }

    /* factor odd primes */
    idx_t p = 3;
    while (p * p <= temp) {
        if (temp % p == 0) {
            idx_t e = 0;
            seq_t pk = 1;
            seq_t sum = 1;
            while (temp % p == 0) {
                ++e;
                temp /= p;
                pk = pk * (seq_t)p;
                sum += pk;
            }
            sigma = sigma * sum;
            tau   = tau * (e + 1);
        }
        p += 2;
    }

    /* remaining prime factor > sqrt(original n) */
    if (temp > 1) {
        seq_t pk = 1;
        seq_t sum = 1 + (seq_t)temp;   /* 1 + p */
        sigma = sigma * sum;
        tau   = tau * 2;               /* exponent 1 */
    }

    return (seq_t)n + sigma - tau;
}
