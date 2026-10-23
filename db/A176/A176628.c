/*
 * A176628  a(n) = prime(n) - n*(-1)^prime(n).
 * https://oeis.org/A176628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) - n*(-1)^prime(n) (i.e. a(1)=1, for n≥2:
 *            a(n)=prime(n)+n)
 * Range:     u8 n <= 46, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Dynamically grow a list of primes up to the n‑th prime */
    static uint32_t *primes = NULL;
    static size_t primes_len = 0;
    static size_t primes_cap = 0;
    static uint32_t last_checked = 1;

    /* Ensure the primes array is large enough */
    if (primes_len == 0) {
        primes_cap = 128;
        primes = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
        if (!primes) return 0; /* allocation failed */
    }

    /* Generate primes until we have at least n primes */
    while (primes_len < n) {
        uint32_t candidate = last_checked + 1;
        if (candidate < 2) candidate = 2;

        /* Find next prime candidate */
        while (1) {
            bool prime_flag = true;
            for (size_t i = 0; i < primes_len; ++i) {
                uint32_t p = primes[i];
                uint64_t sq = (uint64_t)p * (uint64_t)p;
                if (sq > candidate) break;
                if (candidate % p == 0) { prime_flag = false; break; }
            }
            if (prime_flag) break;
            ++candidate;
        }

        /* Store the new prime */
        if (primes_len == primes_cap) {
            primes_cap <<= 1;
            uint32_t *tmp = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
            if (!tmp) return 0; /* reallocation failed */
            primes = tmp;
        }
        primes[primes_len++] = candidate;
        last_checked = candidate;
    }

    /* Retrieve the required prime and compute a(n) */
    uint32_t p = primes[n - 1];
    if (p == 2) { /* even prime case */
        return (seq_t)p - (seq_t)n;
    } else { /* odd prime case */
        return (seq_t)p + (seq_t)n;
    }
}
