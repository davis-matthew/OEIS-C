/*
 * A105562  a(n) is the prime whose index is the greatest prime factor of n,
 *          for n >1; a(1)=2.
 * https://oeis.org/A105562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   a(n) = prime(A006530(n)) (prime of the greatest prime factor of
 *            n)
 * Range:     u8 n <= 58, u16 n <= 6546, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   max_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/max_prime_factor.h"   /* seq_t max_prime_factor(idx_t n); */

/* Static storage for generated primes (1‑based indexing internally) */
static uint32_t *primes = NULL;         /* array of primes, primes[0] = 2 */
static idx_t     prime_count = 0;       /* number of primes stored */
static idx_t     prime_cap   = 0;       /* allocated capacity */

/* Ensure that primes[0..(needed_index‑1)] exist (needed_index >= 1). */
static void ensure_primes(idx_t needed_index)
{
    if (prime_count >= needed_index)
        return;

    /* Allocate or grow the primes array if necessary. */
    if (prime_cap < needed_index) {
        while (prime_cap < needed_index)
            prime_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
        uint32_t *tmp = (uint32_t *)realloc(primes, prime_cap * sizeof(uint32_t));
        if (!tmp) /* out of memory – just return, caller will fail gracefully */
            return;
        primes = tmp;
    }

    /* Generate primes until we have enough. */
    for (uint32_t candidate = (prime_count == 0) ? 2 : primes[prime_count - 1] + 1;
         prime_count < needed_index;
         ++candidate) {
        bool is_prime = true;
        for (idx_t i = 0; i < prime_count; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * (uint64_t)p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            primes[prime_count++] = candidate;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* a(1) = 2 by definition */
    if (n <= 1)
        return (seq_t)2;

    /* Step 1: greatest prime factor of n */
    seq_t lpf_seq = max_prime_factor(n);
    idx_t lpf = (idx_t)lpf_seq;   /* should fit into idx_t */

    /* Safety check – for n > 1 the lpf should be at least 2 */
    if (lpf < 2)
        return (seq_t)2;

    /* Step 2: generate necessary primes */
    ensure_primes(lpf);

    /* Step 3: return the lpf‑th prime (1‑based indexing) */
    return (seq_t)primes[lpf - 1];
}
