/*
 * A068678  Numbers that yield a prime when a 9 is prefixed or suffixed to
 *          them.
 * https://oeis.org/A068678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n-th positive integer m such that 9m and m9 are
 *            simultaneously prime. Implemented by a brute‑force search with a
 *            precomputed prime sieve up to 20 000 000
 * Range:     u8 n <= 17, u16 n <= 1967, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bit_is_set, bit_set
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bit_is_set.h"
#include "../utils/bit_set.h"

/* Prime sieve parameters */
#define PRIME_LIMIT 20000000UL          /* must be large enough for all needed 9m and m9 */
static unsigned char prime_bits[(PRIME_LIMIT >> 3) + 1];
static bool primes_prepared = false;

/* Prepare the sieve once.  All primes up to PRIME_LIMIT are marked. */
static void prepare_primes(void)
{
    if (primes_prepared) return;

    /* Clear bit array */
    for (uint32_t i = 0; i < (PRIME_LIMIT >> 3) + 1; ++i) prime_bits[i] = 0;

    /* Mark 0 and 1 as composite */
    bit_set(prime_bits, 0);
    bit_set(prime_bits, 1);

    /* Sieve of Eratosthenes */
    for (uint32_t p = 2; (uint64_t)p * (uint64_t)p <= PRIME_LIMIT; ++p) {
        if (bit_is_set(prime_bits, p)) continue;
        for (uint64_t q = (uint64_t)p * (uint64_t)p; q <= PRIME_LIMIT; q += p) {
            bit_set(prime_bits, (uint32_t)q);
        }
    }
    primes_prepared = true;
}

/* Check primality for numbers that fit in PRIME_LIMIT. */
static inline bool is_prime_small(idx_t x)
{
    if (x > PRIME_LIMIT) return false;
    return !bit_is_set(prime_bits, (uint32_t)x);
}

/* --- Sequence storage --- */
static idx_t            next_candidate = 1;      /* next integer to test */
static size_t           cache_len     = 0;       /* number of terms stored */
static size_t           cache_cap     = 0;       /* allocated capacity */
static seq_t *          cache_terms   = NULL;    /* dynamic array of terms */

/* Ensure cache has enough space for at least new_cap elements. */
static void ensure_cache(size_t new_cap)
{
    if (cache_cap >= new_cap) return;
    size_t new_capacity = cache_cap ? cache_cap * 2 : 4;
    if (new_capacity < new_cap) new_capacity = new_cap;
    seq_t *new_ptr = (seq_t *)realloc(cache_terms, new_capacity * sizeof(seq_t));
    if (!new_ptr) abort();          /* out of memory */
    cache_terms = new_ptr;
    cache_cap = new_capacity;
}

/* Return the n-th term of the sequence (1‑indexed). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */

    /* Prepare primes once */
    prepare_primes();

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        idx_t m = next_candidate++;

        /* Quick filter: last digit must be 1, 3, 7 or 9, and m not divisible by 3. */
        uint8_t ld = m % 10U;
        if (ld != 1U && ld != 3U && ld != 7U && ld != 9U) continue;
        if (m % 3U == 0U) continue;

        /* Compute number of decimal digits of m. */
        int d = 0;
        idx_t tmp = m;
        do { ++d; tmp /= 10U; } while (tmp);

        /* 10^d as idx_t.  For m up to ~1 000 000, this stays in 64 bits. */
        idx_t pow10 = 1;
        for (int i = 0; i < d; ++i) pow10 *= 10U;

        /* Compute the prefixed and suffixed numbers. */
        idx_t prefIdx = 9U * pow10 + m;          /* 9 · 10^d + m  */
        idx_t suffIdx = m * 10U + 9U;            /* m · 10 + 9   */

        /* If the numbers are outside our sieve range we skip the candidate. */
        if (prefIdx > PRIME_LIMIT || suffIdx > PRIME_LIMIT) continue;

        /* Test both for primality. */
        if (!is_prime_small(prefIdx) || !is_prime_small(suffIdx)) continue;

        /* Store the candidate. */
        ensure_cache(cache_len + 1);
        cache_terms[cache_len++] = (seq_t)m;
    }

    return cache_terms[n - 1];
}
