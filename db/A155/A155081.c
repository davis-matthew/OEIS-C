/*
 * A155081  Primes p such that the largest digit of the concatenation of p and
 *          the p-th prime is a prime.
 * https://oeis.org/A155081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   concat_parts, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A155081: primes p such that the largest digit of the decimal concatenation
   of p and the p‑th prime is itself prime.  The function returns the n‑th
   term of that sequence (1‑based). */

#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime.h"      /* bool is_prime(idx_t); */
#include "../utils/concat_parts.h"  /* seq_t concat_parts(idx_t, idx_t); */

/* Return the n‑th term of sequence A155081.  Cache is built on demand
   and reused for subsequent calls. */
seq_t a(idx_t n)
{
    /*----- Persistent state for memoisation -----*/
    static seq_t *cache      = NULL;
    static size_t cache_cnt  = 0;
    static size_t cache_cap  = 0;

    /*----- Prime sieve state -----*/
    static uint64_t *primes             = NULL;
    static size_t   primes_cnt          = 0;
    static size_t   primes_cap          = 0;
    static uint64_t   nextPrimeCandidate = 2;   /* candidate for next prime */
    static size_t   primeIdx            = 0;     /* index of next prime candidate p */

    /*----- Initialise caches when first called -----*/
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    if (!primes) {
        primes_cap = 64;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    }

    /* Return a previously computed term if available. */
    if (n > 0 && (size_t)n <= cache_cnt)
        return cache[n - 1];

    /* Main generation loop: produce terms until we have at least n of them. */
    while (cache_cnt < n) {
        /*--- Ensure we have the prime at index primeIdx (zero‑based). ---*/
        while (primes_cnt <= primeIdx) {
            /* Find the next prime number. */
            while (!is_prime((idx_t)nextPrimeCandidate))
                ++nextPrimeCandidate;
            /* Store it. */
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_cnt++] = nextPrimeCandidate;
            ++nextPrimeCandidate;
        }

        /*--- Candidate prime p for this iteration. ---*/
        uint64_t p_val = primes[primeIdx++];
        /*--- Ensure we have the p_val‑th prime (1‑based) available. ---*/
        while (primes_cnt < p_val) {
            while (!is_prime((idx_t)nextPrimeCandidate))
                ++nextPrimeCandidate;
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_cnt++] = nextPrimeCandidate;
            ++nextPrimeCandidate;
        }
        /* Retrieve the p‑th prime. */
        seq_t pth_prime = (seq_t)primes[p_val - 1];

        /* Concatenate p and its p‑th prime. */
        seq_t concat = concat_parts((idx_t)p_val, (idx_t)pth_prime);

        /* Find the largest decimal digit of the concatenation. */
        seq_t tmp = concat;
        int largest = 0;
        while (tmp > 0) {
            int d = (int)(tmp % (seq_t)10);
            if (d > largest) largest = d;
            tmp /= (seq_t)10;
        }

        /* If the largest digit is prime (2,3,5,7) record this p. */
        if (largest == 2 || largest == 3 || largest == 5 || largest == 7) {
            if (cache_cnt == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_cnt++] = (seq_t)p_val;
        }
    }

    return cache[n - 1];
}
