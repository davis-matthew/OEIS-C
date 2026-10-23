/*
 * A145784  Numbers with property that their number of prime factors counted
 *          with multiplicity is a multiple of 3.
 * https://oeis.org/A145784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer whose total number of prime *
 *            factors counted with multiplicity (Ω) is a multiple of 3. *
 *            Offset: 1 (a(1)=1). The sequence is closed under multiplication
 *            * (if m,n are in it, so is m*n)
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Dynamic array of terms.  The element type is seq_t, supplied by the
 * build system. */
static seq_t *terms       = NULL;
static size_t terms_len   = 0;
static size_t terms_cap   = 0;

static void grow_terms(void)
{
    size_t new_cap = terms_cap ? terms_cap << 1 : 16;
    seq_t *new_ptr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_ptr) abort();
    if (terms) {
        memcpy(new_ptr, terms, terms_len * sizeof(seq_t));
        free(terms);
    }
    terms      = new_ptr;
    terms_cap  = new_cap;
}

/* Dynamic array of primes used for factorisation */
static uint64_t *primes            = NULL;
static size_t primes_len            = 0;
static size_t primes_cap            = 0;
static uint64_t next_prime_candidate = 2;

static void grow_primes(void)
{
    size_t new_cap = primes_cap ? primes_cap << 1 : 16;
    uint64_t *new_ptr = (uint64_t *)malloc(new_cap * sizeof(uint64_t));
    if (!new_ptr) abort();
    if (primes) {
        memcpy(new_ptr, primes, primes_len * sizeof(uint64_t));
        free(primes);
    }
    primes = new_ptr;
    primes_cap = new_cap;
}

/* Ensure that primes contains all primes ≤ limit.
 * The routine keeps a cursor so each prime is generated only once. */
static void ensure_primes_up_to(uint64_t limit)
{
    while (next_prime_candidate <= limit) {
        bool is_prime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            if (p * p > next_prime_candidate) break;
            if (next_prime_candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (primes_len == primes_cap) grow_primes();
            primes[primes_len++] = next_prime_candidate;
        }
        ++next_prime_candidate;
    }
}

/* Compute Ω(n) – number of prime factors of n with multiplicity. */
static int omega64(uint64_t n)
{
    int cnt = 0;
    if (n <= 1) return cnt;
    uint64_t temp = n;
    for (size_t i = 0; i < primes_len && primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        while (temp % p == 0) {
            temp /= p;
            ++cnt;
        }
    }
    if (temp > 1) ++cnt;       /* remaining prime factor */
    return cnt;
}

/* Public sequence generator.  It caches results so repeated calls
 * with arbitrary n are O(1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but harmless */

    /* Start searching from the smallest candidate that has not been
     * examined yet.  When the cache already contains some terms,
     * the next candidate is simply one greater than the last stored
     * term. */
    uint64_t candidate = terms_len ? (uint64_t)terms[terms_len - 1] + 1
                                   : 1;

    /* Expand the cache until it holds the nth term. */
    while (terms_len < (size_t)n) {
        /* Make sure we know all primes up to the current candidate. */
        ensure_primes_up_to(candidate);

        if (omega64(candidate) % 3 == 0) {
            if (terms_len == terms_cap) grow_terms();
            terms[terms_len++] = (seq_t)candidate;
        }
        ++candidate;
    }
    return terms[n - 1];
}
