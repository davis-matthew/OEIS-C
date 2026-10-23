/*
 * A161213  a(n) = Sum_{d|n} Moebius(n/d)*d^(b-1)/phi(n) for b = 18.
 * https://oeis.org/A161213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = J_17(n)/φ(n) = * multiplicative with a(p^e) = p^(16e‑16)
 *            * (p^17‑1)/(p‑1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 15, u128 n <= 239,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses trial‑division factorisation and
 * the supplied big_pow helper for arbitrary-precision exponentiation. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;          /* a(1) = 1 */

    /* factorise n by trial division (sufficient for n ≤ 10^8,
       and acceptable for the problem limits) */
    uint64_t tmp = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   size = 0;

    if (tmp % 2 == 0) {
        uint64_t e = 0;
        while (tmp % 2 == 0) {
            tmp /= 2;
            ++e;
        }
        primes[size] = 2;
        exps[size]   = e;
        ++size;
    }

    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[size] = p;
            exps[size]   = e;
            ++size;
        }
    }

    if (tmp > 1) {
        primes[size] = tmp;
        exps[size]   = 1;
        ++size;
    }

    /* accumulate the product of the prime‑power factors */
    seq_t result = (seq_t)1;

    for (size_t i = 0; i < size; ++i) {
        seq_t p   = (seq_t)primes[i];
        uint64_t e = exps[i];

        /* p^(16(e‑1))  (when e == 1 this is 1) */
        seq_t exp_factor_ex = (seq_t)(16 * (e - 1));
        seq_t pow_factor = big_pow(p, exp_factor_ex);

        /* (p^17-1)/(p-1) = 1 + p + p^2 + … + p^16 */
        seq_t sum = (seq_t)1;
        seq_t term = (seq_t)1;
        for (uint8_t k = 1; k <= 16; ++k) {
            term *= p;
            sum += term;
        }

        /* combine the two parts for this prime power */
        seq_t part = pow_factor * sum;
        result *= part;
    }

    return result;
}
