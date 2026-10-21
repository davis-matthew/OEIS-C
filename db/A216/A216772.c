/*
 * A216772  Primes p such that x^54 = -2 has no solution mod p.
 * https://oeis.org/A216772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A216772: primes p such that x^54 = -2 has no solution mod p */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* is_prime(uint64_t) */
#include "../utils/powmod64.h"   /* powmod64(uint64_t, uint64_t, uint64_t) */

/* compute gcd of two unsigned integers */
static inline uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* sequence function */
seq_t a(idx_t n)
{
    /* static storage for found primes */
    static uint64_t *prime_cache       = NULL;
    static size_t     prime_cache_cnt   = 0;
    static size_t     prime_cache_cap   = 0;
    static uint64_t   candidate          = 3;   /* start from the smallest odd prime */

    /* generate primes until we have at least n terms */
    while (prime_cache_cnt < n) {
        /* skip even numbers */
        if ((candidate & 1ULL) != 0) {
            if (is_prime(candidate)) {
                uint64_t d   = gcd_u64(54ULL, candidate - 1ULL);
                uint64_t exp = (candidate - 1ULL) / d;
                uint64_t pow = powmod64(candidate - 2ULL, exp, candidate);

                /* if -2 is NOT a 54th power residue, keep this prime */
                if (pow != 1ULL) {
                    /* grow cache if needed */
                    if (prime_cache_cnt == prime_cache_cap) {
                        size_t new_cap = (prime_cache_cap == 0) ? 64 : prime_cache_cap << 1;
                        uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
                        if (tmp == NULL) abort();          /* allocation failure */
                        prime_cache     = tmp;
                        prime_cache_cap = new_cap;
                    }
                    prime_cache[prime_cache_cnt++] = candidate;
                }
            }
        }
        /* move to the next odd integer */
        candidate += 2;
    }

    return (seq_t)prime_cache[n - 1];
}
