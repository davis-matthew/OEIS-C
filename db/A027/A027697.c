/*
 * A027697  Odious primes: primes with odd number of 1's in binary expansion.
 * https://oeis.org/A027697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 3684, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the n‑th odious prime (a prime whose binary
 * expansion contains an odd number of 1’s).  It iteratively looks for
 * the next prime, checks the pop‑count parity, and stores any
 * odious prime in a static cache so that calls in any order are
 * handled efficiently.  An internal prime list is maintained; it is
 * used to test primality by simple trial division.  The implementation
 * works for all supported build configurations (uint8_t … arbitrary
 * precision). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* popcount for 64‑bit values */
static inline unsigned int popcount_u64(uint64_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll((unsigned long long)x);
#else
    unsigned int c = 0;
    while (x) { x &= x - 1; ++c; }
    return c;
#endif
}

/* Dynamic array of the odious primes found so far (cache). */
static seq_t *odious_cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t last_checked = 1;   /* last number that was inspected */

/* Dynamic array of all primes discovered, used for trial division. */
static idx_t *prime_list = NULL;
static size_t prime_cap = 0;
static size_t prime_len = 0;

/* Ensure that prime_list has at least new_cap slots. */
static void extend_prime_list(size_t new_cap)
{
    idx_t *tmp = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
    if (!tmp) exit(1);          /* allocation failed */
    prime_list = tmp;
    prime_cap = new_cap;
}

/* Ensure that odious_cache has at least new_cap slots. */
static void extend_cache(size_t new_cap)
{
    seq_t *tmp = (seq_t *)realloc(odious_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);          /* allocation failed */
    odious_cache = tmp;
    cache_cap = new_cap;
}

/* Test if cand (a 64‑bit integer) is prime using the prime_list. */
static bool is_prime_candidate(idx_t cand)
{
    /* handle 1 and 2 separately */
    if (cand <= 1) return false;
    if (cand == 2) return true;
    if ((cand & 1U) == 0) return false;

    for (size_t i = 0; i < prime_len; ++i) {
        idx_t p = prime_list[i];
        if (p * p > cand) break;
        if (cand % p == 0) return false;
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined; guard against zero index */

    /* Discover odious primes until we have at least n of them. */
    while (cache_len < n) {
        /* Find next prime greater than last_checked. */
        idx_t cand = last_checked + 1;
        while (!is_prime_candidate(cand))
            ++cand;

        /* Record the prime in the prime list. */
        if (prime_len == prime_cap) {
            size_t newcap = prime_cap ? (prime_cap << 1) : 8;
            extend_prime_list(newcap);
        }
        prime_list[prime_len++] = cand;

        /* Test odiousness (odd popcount). */
        if ((popcount_u64(cand) & 1U) != 0) {
            /* Store in the cache. */
            if (cache_len == cache_cap) {
                size_t newcap = cache_cap ? (cache_cap << 1) : 8;
                extend_cache(newcap);
            }
            odious_cache[cache_len++] = (seq_t)cand;
        }

        last_checked = cand;  /* next search starts from here. */
    }

    return odious_cache[n - 1];
}
