/*
 * A177334  Largest factor in the factorization of n! over distinct terms of
 *          A050376.
 * https://oeis.org/A177334
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{p^k term from n! factorization} p^k where k is a
 *            power of two not exceeding the exponent of prime p in n!
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"

/* Prime cache */
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_cap   = 0;

/* Ensure the prime list contains all primes <= limit. */
static void ensure_primes(idx_t limit)
{
    if (limit < 2) return;

    uint32_t max_needed = (uint32_t)limit;

    /* First time: initialise the cache with prime 2. */
    if (prime_count == 0) {
        prime_cap = 64;
        prime_list = (uint32_t *)realloc(NULL, prime_cap * sizeof(uint32_t));
        if (!prime_list) return;               /* allocation failure → cannot continue */
        prime_list[prime_count++] = 2;
    }

    uint32_t start = prime_list[prime_count - 1] + 1;
    if (start <= 2) start = 3;                /* after 2 only odd candidates are needed */

    for (uint32_t cand = start; cand <= max_needed; ++cand) {
        if (cand % 2 == 0) continue;          /* skip evens > 2 */

        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t p = prime_list[i];
            if ((uint64_t)p * (uint64_t)p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            if (prime_count >= prime_cap) {
                size_t new_cap = prime_cap * 2;
                uint32_t *tmp = (uint32_t *)realloc(prime_list,
                                                     new_cap * sizeof(uint32_t));
                if (!tmp) return;                /* allocation failure → stop */
                prime_list = tmp;
                prime_cap   = new_cap;
            }
            prime_list[prime_count++] = cand;
        }
    }
}

/* Compute the largest p^k factor of n! where k is a power of two. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                     /* values below offset are undefined */

    ensure_primes(n);

    seq_t best = 0;

    for (size_t i = 0; i < prime_count && prime_list[i] <= n; ++i) {
        uint32_t p = prime_list[i];

        /* Legendre's formula for exponent of p in n! */
        idx_t exp = 0;
        idx_t m   = n / p;
        while (m) {
            exp += m;
            m   /= p;
        }

        /* Split exp into powers of two and compute the corresponding terms. */
        idx_t remaining = exp;
        while (remaining) {
            idx_t t = remaining & -remaining;                 /* lowest 1‑bit */
            seq_t term = pow_uint((uint64_t)p, t);
            if (term > best) best = term;
            remaining -= t;
        }
    }

    return best;
}
