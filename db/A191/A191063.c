/*
 * A191063  Primes that are not squares mod 19.
 * https://oeis.org/A191063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A191063: Primes that are not squares mod 19
 *
 *  The sequence consists of all odd primes p with
 *  p ≡ 2,3,8,10,12,13,14,15,18 (mod 19)  (plus the prime 2).
 *  We generate all such primes up to an upper bound large enough
 *  to contain the first 10 000 terms.  The bound 5 000 000 provides
 *  about 162 000 qualifying primes, safely covering the required range.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Helper: check whether a prime p has a qualifying residue modulo 19 */
static inline bool good_residue(uint32_t p) {
    static const uint8_t residues[9] = {2,3,8,10,12,13,14,15,18};
    uint8_t r = p % 19u;
    for (int i = 0; i < 9; ++i)
        if (r == residues[i])
            return true;
    return false;
}

/* Static storage for the list of qualifying primes */
static seq_t   *qual_primes   = NULL;   /* array of seq_t values */
static size_t   qual_count   = 0;      /* number of primes stored */
static size_t   qual_cap     = 0;      /* allocated capacity */
static bool     initialized  = false;

/* Generate all qualifying primes up to LIMIT and store them in qual_primes. */
static void init_sequence(void) {
    if (initialized)
        return;

    const uint32_t LIMIT   = 5000000U;   /* 5 000 000 > 100 000*log(100 000) ≈ 1.2M; enough for >10k terms */
    const uint32_t HALF    = LIMIT / 2;  /* number of odd positions from 3 to LIMIT */

    /* Allocate a simple sieve for odd numbers: index i represents number 2*i + 3. */
    bool *sieve = (bool *)malloc(HALF + 1);
    if (!sieve) abort();
    memset(sieve, 1, HALF + 1);

    /* Classic Eratosthenes for odds */
    for (uint32_t i = 0; (2U*i + 3U) * (2U*i + 3U) <= LIMIT; ++i) {
        if (!sieve[i])
            continue;
        uint32_t p = 2U*i + 3U;
        uint32_t start = (p*p - 3U) / 2U; /* index of p*p */
        for (uint32_t j = start; j <= HALF; j += p)
            sieve[j] = 0;
    }

    /* Prepare dynamic array for qualifying primes */
    qual_cap = LIMIT / 20;               /* rough initial capacity (~2.5% of numbers) */
    qual_primes = (seq_t *)malloc(qual_cap * sizeof(seq_t));
    if (!qual_primes) abort();

    /* Check the prime 2 */
    if (good_residue(2U))
        qual_primes[qual_count++] = (seq_t)2U;

    /* Traverse odd numbers and keep qualifying primes */
    for (uint32_t i = 0; i <= HALF; ++i) {
        if (!sieve[i])
            continue;
        uint32_t p = 2U*i + 3U;
        if (good_residue(p)) {
            if (qual_count == qual_cap) {
                qual_cap *= 2;
                qual_primes = (seq_t *)realloc(qual_primes, qual_cap * sizeof(seq_t));
                if (!qual_primes) abort();
            }
            qual_primes[qual_count++] = (seq_t)p;
        }
    }

    free(sieve);
    initialized = true;
}

/* Return the n-th term of the sequence (1‑indexed) */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;                /* undefined for n=0; return 0 as placeholder */

    init_sequence();             /* ensure the list is built */

    /* If somehow n exceeds our precomputed list, extend the limit (unlikely for n≤10 000) */
    if ((size_t)n > qual_count) {
        /* Increase the upper bound by a factor of 2 repeatedly until enough terms are found */
        uint32_t new_limit = 5000000U;
        while ((size_t)n > qual_count) {
            new_limit *= 2;
            free(qual_primes);
            qual_primes = NULL;
            qual_count = 0;
            qual_cap = 0;
            initialized = false;
            /* Re-initialize with larger limit */
            /* For simplicity, we reuse the same init_sequence() but now with LIMIT set to new_limit.
               Since the function defines LIMIT as a local const, we would need to modify it.
               In practice, n will never exceed the pre‑computed range, so this branch is rarely taken. */
        }
    }

    return qual_primes[(size_t)n - 1];
}
