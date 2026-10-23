/*
 * A181819  Prime shadow of n: a(1) = 1; for n>1, if n = Product
 *          prime(i)^e(i), then a(n) = Product prime(e(i)).
 * https://oeis.org/A181819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(1) = 1; for n > 1, if n = ∏ p_i^e_i, then a(n) = ∏ prime(e_i)
 *            (prime(k) = k‑th prime)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factor n into primes and exponents */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        /* prime(e_i) */
        seq_t prime_e = (seq_t)get_prime((idx_t)exps[i]);
        result = result * prime_e;
    }
    return result;
}
