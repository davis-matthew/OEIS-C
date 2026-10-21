/*
 * A205693  Prime(A205691(n)), the n-th number s(k) such that 6 divides
 *          s(k)-s(j) for some j<k, where s(j)=prime(j).
 * https://oeis.org/A205693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p>3 that has a preceding prime q≡p (mod
 *            6); each prime p is listed that many times as the number of
 *            earlier primes with the same residue modulo 6
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes all primes up to a fixed bound large enough to
   cover at least the first 10000 terms of the sequence, and then scans those
   primes to find the desired n‑th term. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Fixed upper bound – enough to generate >25 000 terms of the sequence,
   which covers the required n up to about 10 000. */
#define PRIME_LIMIT 2000

/* Static storage for primes; initialized on first use. */
static uint32_t primes[3000];
static size_t prime_cnt = 0;
static bool primes_inited = false;

/* Simple Sieve of Eratosthenes to fill primes[] up to PRIME_LIMIT. */
static void init_primes(void)
{
    bool *is_prime = (bool*)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    if (!is_prime) return;                  /* allocation failure – should not happen */

    for (size_t i = 0; i <= PRIME_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) primes[prime_cnt++] = (uint32_t)i;

    free(is_prime);
    primes_inited = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!primes_inited) init_primes();

    /* Ensure n is at least 1. */
    if (n == 0) return 0;

    /* Counters for residues modulo 6. Only residues 1 and 5 are used. */
    size_t residue_cnt[6] = {0};

    idx_t remaining = n;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if (p <= 3) continue;            /* skip 2 and 3 */

        int r = (int)(p % 6);
        size_t repeats = residue_cnt[r];

        if (repeats > 0) {
            if (remaining <= repeats) {
                /* Found the desired term. */
                return (seq_t)p;
            } else {
                remaining -= repeats;
            }
        }
        /* Update counter for this residue for subsequent primes. */
        residue_cnt[r] += 1;
    }

    /* If we reach here, our bound was insufficient – increase and recompute. */
    /* This should not happen for n ≤ 10000 with PRIME_LIMIT=2000. */
    size_t new_limit = PRIME_LIMIT * 2;
    while (true) {
        /* Reallocate prime array with larger bound. */
        free(primes); prime_cnt = 0;
        bool *is_prime = (bool*)malloc((new_limit + 1) * sizeof(bool));
        if (!is_prime) return 0;
        for (size_t i = 0; i <= new_limit; ++i) is_prime[i] = true;
        is_prime[0] = is_prime[1] = false;
        for (size_t i = 2; i * i <= new_limit; ++i)
            if (is_prime[i])
                for (size_t j = i * i; j <= new_limit; j += i)
                    is_prime[j] = false;
        for (size_t i = 2; i <= new_limit; ++i)
            if (is_prime[i]) primes[prime_cnt++] = (uint32_t)i;
        free(is_prime);

        /* Reset counters and remaining. */
        for (size_t i = 0; i < 6; ++i) residue_cnt[i] = 0;
        remaining = n;

        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = primes[i];
            if (p <= 3) continue;
            int r = (int)(p % 6);
            size_t repeats = residue_cnt[r];
            if (repeats > 0) {
                if (remaining <= repeats) return (seq_t)p;
                remaining -= repeats;
            }
            residue_cnt[r] += 1;
        }

        /* Double the limit for next attempt. */
        new_limit *= 2;
        if (new_limit > 10000000) break; /* safety guard */
    }

    /* Should never reach here. */
    return 0;
}
