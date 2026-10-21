/*
 * A092622  Primes with exactly two prime digits.
 * https://oeis.org/A092622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   ensure_uint64_cap, has_two_prime_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A092622: primes with exactly two prime digits (2,3,5,7).
   The function generates primes incrementally, keeps the primes found
   in a dynamically growing array, and collects the ones whose decimal
   representation contains precisely two of the prime digits.
   It caches the result so that subsequent calls for any n are served
   from the cache.  The implementation is O(n * sqrt(a(n))) and
   comfortably fits in the required run‑time for n up to ~10000. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/ensure_uint64_cap.h"
#include "../utils/has_two_prime_digits.h"

/* Helper: ensures that an array of seq_t has at least 'need' slots. */
static void ensure_seq_cap(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap) ? (*cap) * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* Return true if cand is prime, using the primes that have already been
   found and stored in primes[0..primes_len-1]. */
static bool is_prime_uint64(uint64_t cand, const uint64_t *primes, size_t primes_len)
{
    if (cand <= 1) return false;
    if (cand == 2) return true;
    if ((cand & 1) == 0) return false;
    uint64_t limit = (uint64_t)(sqrt((double)cand) + 1.0);
    for (size_t i = 0; i < primes_len && primes[i] <= limit; ++i) {
        if (cand % primes[i] == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* no defined value for a(0) */

    /* Persistent data across calls. */
    static uint64_t *primes = NULL;
    static size_t primes_len = 0;
    static size_t primes_cap = 0;
    static seq_t  *terms  = NULL;
    static size_t  terms_len  = 0;
    static size_t  terms_cap  = 0;
    static uint64_t current = 1; /* last candidate tested */

    /* Initialise static state once. */
    if (!primes) {
        ensure_uint64_cap(&primes, &primes_cap, 16);
        primes[0] = 2;
        primes_len = 1;
    }
    if (!terms) {
        ensure_seq_cap(&terms, &terms_cap, 16);
    }

    /* Generate primes and answers until we have n terms. */
    while (terms_len < (size_t)n) {
        /* Next candidate: after 2 only odd numbers. */
        uint64_t cand = (current < 2) ? 2 : current + 1;
        if (cand > 2 && cand % 2 == 0) ++cand;

        if (is_prime_uint64(cand, primes, primes_len)) {
            /* Store new prime. */
            ensure_uint64_cap(&primes, &primes_cap, primes_len + 1);
            primes[primes_len++] = cand;

            if (has_two_prime_digits(cand)) {
                /* Store the term. */
                ensure_seq_cap(&terms, &terms_cap, terms_len + 1);
                terms[terms_len++] = (seq_t)cand;
            }
        }
        current = cand;
    }

    return terms[n - 1];
}
