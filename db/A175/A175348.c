/*
 * A175348  Last digit of p^p, where p is the n-th prime.
 * https://oeis.org/A175348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the last decimal digit of prime(n)^prime(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n < 1, harmless */

    static uint32_t *primes = NULL;
    static size_t primes_count = 0;
    static idx_t max_n = 0;

    if (n > max_n) {
        free(primes);
        primes = NULL;
        primes_count = 0;
        sieve_first_n_primes(n, &primes, &primes_count);
        max_n = n;
    }

    uint32_t p = primes[n - 1];      /* nth prime (1‑indexed) */

    /* Compute p^p mod 10 using binary exponentiation on 64‑bit integers. */
    uint64_t base = (uint64_t)p % 10ull;
    uint64_t exp  = (uint64_t)p;
    uint64_t res  = 1ull;
    const uint64_t mod = 10ull;

    while (exp != 0) {
        if (exp & 1ull) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ull;
    }

    return (seq_t)res;                /* last digit of p^p */
}
