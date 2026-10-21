/*
 * A226367  Multiplicative order of the (n+1)-st prime modulo the n-th prime.
 * https://oeis.org/A226367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 6544, u32 n <= 9813, u64 n <= 9813,
 *            u128 n <= 9813, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative order of p(n+1) modulo p(n)
   a(n) = smallest positive m with p(n+1)^m ≡ 1 (mod p(n)), n ≥ 1
   where p(k) is the k‑th prime.
   The function uses trial‑division to factor p(n)−1 and reduces the
   order by its prime divisors using fast modular exponentiation.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k) */

static seq_t mod_pow_uint64(seq_t base, uint64_t exp, seq_t mod)
{
    uint64_t b  = (uint64_t)base;
    uint64_t m  = (uint64_t)mod;
    uint64_t result = 1;

    while (exp > 0) {
        if (exp & 1ULL) {
            __uint128_t prod = (__uint128_t)result * b;
            result = (uint64_t)(prod % m);
        }
        exp >>= 1ULL;
        if (exp) {
            __uint128_t prod = (__uint128_t)b * b;
            b = (uint64_t)(prod % m);
        }
    }
    return (seq_t)result;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1 */

    /* Retrieve the n‑th and (n+1)‑st primes */
    uint32_t p_n   = get_prime(n);
    uint32_t p_np1 = get_prime(n + 1);

    uint64_t mod  = (uint64_t)p_n;
    uint64_t base = (uint64_t)(p_np1 % mod);

    /* The multiplicative order divides p_n−1 */
    uint64_t order = mod - 1;

    /* Factorize order into its unique prime divisors.
       array size 64 is ample for the range of p_n we need. */
    uint64_t factors[64];
    size_t   fcnt = 0;
    uint64_t temp = order;

    for (uint64_t d = 2; d * d <= temp; ++d) {
        if (temp % d == 0) {
            factors[fcnt++] = d;
            while (temp % d == 0) temp /= d;
        }
    }
    if (temp > 1) factors[fcnt++] = temp;   /* remaining prime factor */

    /* Reduce order by dividing out each prime factor while the condition
       remains true.  This yields the smallest exponent m. */
    for (size_t i = 0; i < fcnt; ++i) {
        uint64_t q = factors[i];
        while (order % q == 0) {
            uint64_t candidate = order / q;
            if (mod_pow_uint64(base, candidate, mod) == 1)
                order = candidate;
            else
                break;
        }
    }

    return (seq_t)order;
}
