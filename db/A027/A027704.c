/*
 * A027704  Numbers k such that the k-th prime has an even number of 1's in
 *          its binary expansion and the (k+1)st prime has an odd number of
 *          1's.
 * https://oeis.org/A027704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generating the OEIS A027704 sequence.
   a(n) = k   where k is the index of a prime p_k such that
   popcount(p_k) is even and popcount(p_{k+1}) is odd. */

/* Preliminaries */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper to count ones in a 64‑bit word. */
static unsigned popcount64(uint64_t x)
{
    unsigned c = 0;
    while (x) { x &= x - 1; ++c; }
    return c;
}

/* Simple primality test using the already known primes. */
static bool is_prime_uint64(uint64_t n, uint64_t *primes, size_t primes_cnt)
{
    if (n < 2) return false;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* The main sequence function.  It keeps a cache of primes and the
   sequence terms so that subsequent calls are fast and may be in any
   order. */
seq_t a(idx_t n)
{
    /* For safety, if n is 0 return 0. */
    if (n == 0) return (seq_t)0;

    /* Persistent cache across calls. */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0, primes_cap = 0;
    static seq_t *terms  = NULL;   /* sequence elements */
    static size_t terms_cnt = 0, terms_cap = 0;

    /* Initialisation on first call. */
    if (primes == NULL) {
        primes_cap = 32;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        if (!primes) return (seq_t)0;   /* allocation failed – unlikely */

        terms_cap = 32;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) return (seq_t)0;

        /* First prime is 2. */
        primes[0] = 2;
        primes_cnt = 1;

        /* Parity of the first prime.  2 has one '1' => odd. */
        bool prev_even = (popcount64(2) % 2 == 0);

        /* Now start generating further primes. */
        uint64_t cand = 3;
        size_t i = 1; /* index of current prime (1-based). */
        while (terms_cnt < (size_t)n) {
            /* Expand primes array if needed. */
            if (primes_cnt >= primes_cap) {
                primes_cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!tmp) return (seq_t)0;  /* allocation failed */
                primes = tmp;
            }

            /* Find next prime. */
            while (!is_prime_uint64(cand, primes, primes_cnt)) cand += 2;
            primes[primes_cnt++] = cand;
            ++i; /* new prime has index i */

            bool curr_even = (popcount64(cand) % 2 == 0);

            /* If previous prime had even popcount and the new prime
               has odd popcount, we find a term. */
            if (prev_even && !curr_even) {
                /* Ensure terms array capacity. */
                if (terms_cnt >= terms_cap) {
                    terms_cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                    if (!tmp) return (seq_t)0;
                    terms = tmp;
                }
                terms[terms_cnt++] = (seq_t)(i - 1); /* k = previous index */
            }

            prev_even = curr_even;   /* update for next iteration */
            cand += 2;
        }
    } else {
        /* Cache already exists – just make sure it contains n terms. */
        while (terms_cnt < (size_t)n) {
            /* Expand primes array if needed. */
            if (primes_cnt >= primes_cap) {
                primes_cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!tmp) return (seq_t)0;
                primes = tmp;
            }

            /* Find next prime. */
            static bool prev_even; /* parity of previous prime; stored earlier */
            static uint64_t *prev_prime = NULL; /* points to last prime */
            static bool initialized_prev = false;

            /* On first call after init we need to set prev_even. */
            if (!initialized_prev) {
                prev_even = (popcount64(primes[primes_cnt - 1]) % 2 == 0);
                initialized_prev = true;
            }

            /* Candidate starts from last prime + 2 (odd numbers). */
            static uint64_t cand = 0;
            if (cand == 0) cand = primes[primes_cnt - 1] + 2;
            else cand += 2;

            while (!is_prime_uint64(cand, primes, primes_cnt)) cand += 2;
            primes[primes_cnt++] = cand;
            size_t i = primes_cnt; /* current index (1‑based) */

            bool curr_even = (popcount64(cand) % 2 == 0);

            if (prev_even && !curr_even) {
                if (terms_cnt >= terms_cap) {
                    terms_cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                    if (!tmp) return (seq_t)0;
                    terms = tmp;
                }
                terms[terms_cnt++] = (seq_t)(i - 1);
            }

            prev_even = curr_even;
        }
    }

    return terms[n - 1];
}
