/*
 * A040992  Primes p such that x^6 = 2 has a solution mod p.
 * https://oeis.org/A040992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that x^6 = 2 has a solution mod p
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

/* Euclidean GCD for uint64_t */
static uint64_t ugcd_uint64(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Resize or allocate a dynamic array of elements of given size */
static void *grow_array(void **ptr, size_t *cnt, size_t *cap, size_t elem_size)
{
    if (*cnt >= *cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        void *tmp = realloc(*ptr, new_cap * elem_size);
        if (!tmp) abort();          /* out of memory */
        *ptr = tmp;
        *cap = new_cap;
    }
    return *ptr;
}

/* Sequence function */
seq_t a(idx_t n)
{
    /* Static state: primes list, good primes list, and current candidate */
    static seq_t *good_primes = NULL;
    static size_t good_cnt = 0, good_cap = 0;
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0, primes_cap = 0;
    static uint64_t candidate = 2;

    /* Compute until we have found the nth good prime */
    while (good_cnt < (size_t)n) {
        /* Primality test for the current candidate */
        bool is_prime = true;
        if (candidate > 2) {
            uint64_t sq = candidate;
            for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= sq; ++i) {
                if (sq % primes[i] == 0) { is_prime = false; break; }
            }
        }

        if (is_prime) {
            /* Store the prime */
            grow_array((void **)&primes, &primes_cnt, &primes_cap, sizeof(uint64_t));
            primes[primes_cnt++] = candidate;

            /* Check whether the prime satisfies the sixth‑power condition */
            bool good = false;
            if (candidate == 2) {
                good = true;                     /* 0^6 ≡ 2 (mod 2) */
            } else {
                uint64_t g = ugcd_uint64(6ULL, candidate - 1ULL);
                uint64_t exp = (candidate - 1ULL) / g;
                seq_t val = mod_pow(2, exp, candidate);
                if (val == 1) good = true;
            }

            if (good) {
                grow_array((void **)&good_primes, &good_cnt, &good_cap, sizeof(seq_t));
                good_primes[good_cnt++] = candidate;
            }
        }

        /* Advance to the next candidate */
        if (candidate == 2)
            candidate = 3;
        else
            candidate += 2;
    }

    return good_primes[n - 1];
}
