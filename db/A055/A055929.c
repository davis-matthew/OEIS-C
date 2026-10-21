/*
 * A055929  Euler totient function of the factorial of prime(n).
 * https://oeis.org/A055929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = φ(prime(n)!) (Euler totient of the factorial of the nth
 *            prime)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   build_primes_upto, get_prime, mul_small, pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/pow_uint.h"
#include "../utils/mul_small.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Obtain the nth prime (1‑based). */
    uint32_t p = get_prime(n);

    /* Build an array of all primes ≤ p. */
    idx_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto((idx_t)p, &primes, &prime_cnt);

    /* Product over all primes dividing p! using Euler’s product for φ. */
    seq_t result = (seq_t)1;

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        idx_t q = primes[i];

        /* Legendre's formula for exponent of q in p!. */
        idx_t e = 0;
        idx_t power = q;
        while (power <= (idx_t)p)
        {
            e += (idx_t)p / power;
            if (power > (idx_t)p / q) break;   /* prevent overflow of power */
            power *= q;
        }

        /* Contribution to φ(p!): q^(e‑1) * (q‑1). */
        seq_t term = pow_uint((uint64_t)q, e - 1);
        term = mul_small(term, (unsigned int)(q - 1));

        result = seq_mul(result, term);
    }

    free(primes);
    return result;
}
