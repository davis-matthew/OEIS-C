/*
 * A106145  Semiprimes (mod 9).
 * https://oeis.org/A106145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-th semiprime) mod 9
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   ensure_seq_cap, ensure_uint32_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A semiprime is a product of two primes (not necessarily distinct). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ensure_seq_cap.h"
#include "../utils/ensure_uint32_cap.h"

/* Return the n-th semiprime modulo 9.  n is 1‑based. */
seq_t a(idx_t n)
{
    /* static storage for primes and semiprimes */
    static uint32_t *prime_list = NULL;
    static size_t prime_cap     = 0;
    static size_t prime_len     = 0;

    static seq_t *semi_list     = NULL;
    static size_t semi_cap      = 0;
    static size_t semi_len      = 0;

    /* next integer to test for being prime or semiprime */
    static uint64_t current = 4;

    /* initialise prime list with 2 and 3 if needed */
    if (prime_len == 0) {
        ensure_uint32_cap(&prime_list, &prime_cap, 16);
        prime_list[0] = 2;
        prime_list[1] = 3;
        prime_len = 2;
    }

    size_t target = (size_t)n;

    /* generate semiprimes until we have at least n of them */
    while (semi_len < target) {
        uint64_t x = current++;
        bool is_prime = true;
        uint32_t first_factor = 0;

        /* trial division by known primes */
        for (size_t i = 0; i < prime_len; ++i) {
            uint32_t p = prime_list[i];
            uint64_t p2 = (uint64_t)p * (uint64_t)p;
            if (p2 > x) break;
            if (x % p == 0) {
                is_prime = false;
                first_factor = p;
                break;
            }
        }

        if (is_prime) {                /* x is a new prime */
            ensure_uint32_cap(&prime_list, &prime_cap, prime_len + 1);
            prime_list[prime_len++] = (uint32_t)x;
            continue;                  /* primes are not semiprimes */
        }

        /* x is composite: check whether it is a semiprime */
        uint64_t q = x / first_factor;
        bool q_is_prime = true;
        for (size_t j = 0; j < prime_len; ++j) {
            uint32_t p = prime_list[j];
            uint64_t p2 = (uint64_t)p * (uint64_t)p;
            if (p2 > q) break;
            if (q % p == 0) {
                q_is_prime = false;
                break;
            }
        }

        if (q_is_prime) {
            ensure_seq_cap(&semi_list, &semi_cap, semi_len + 1);
            semi_list[semi_len++] = (seq_t)(x % 9);
        }
    }

    return semi_list[target - 1];
}
