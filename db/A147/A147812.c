/*
 * A147812  Primes prime(n) such that prime(n+1) - prime(n) > prime(n+2) -
 *          prime(n+1).
 * https://oeis.org/A147812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p such that gap(p, next_prime(p)) >
 *            gap(next_prime(p), next_prime(next_prime(p))) (gap = difference)
 * Range:     u8 n <= 23, u16 n <= 3098, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

/* Cache of qualifying primes */
static seq_t *qual_cache    = NULL;
static size_t qual_cap     = 0;
static idx_t  qual_len     = 0;

/* List of all generated primes (type idx_t to avoid overflow) */
static idx_t *primes       = NULL;
static size_t primes_cap   = 0;
static idx_t  primes_len   = 0;
static uint64_t last_prime = 0;

/* The index (0‑based) of the prime that is currently being tested */
static idx_t test_prime_idx = 0;

/* Ensure that the prime list has at least `need` elements (need ≥ 1). */
static void ensure_primes(idx_t need)
{
    if (need <= primes_len) return;
    /* Resize if necessary */
    while (primes_cap < (size_t)need) {
        size_t new_cap = primes_cap ? primes_cap << 1 : 8;
        if (new_cap < (size_t)need) new_cap = need;
        idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
        if (!tmp) exit(1); /* Out of memory */
        primes = tmp;
        primes_cap = new_cap;
    }
    /* Append missing primes */
    while (primes_len < need) {
        last_prime = next_prime(last_prime);
        primes[primes_len++] = last_prime;
    }
}

/* Ensure that the qualification cache can hold `need` elements. */
static void ensure_cache(idx_t need)
{
    if ((size_t)need <= qual_cap) return;
    size_t new_cap = qual_cap ? qual_cap << 1 : 8;
    if (new_cap < (size_t)need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(qual_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    qual_cache = tmp;
    qual_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Cache hit? */
    if (n <= qual_len) return qual_cache[n - 1];

    /* Generate qualifying primes until we have the nth one. */
    while (qual_len < n) {
        /* Need at least three primes starting at test_prime_idx. */
        idx_t required = test_prime_idx + 3;
        ensure_primes(required);

        idx_t p_cur   = primes[test_prime_idx];
        idx_t p_next  = primes[test_prime_idx + 1];
        idx_t p_next2 = primes[test_prime_idx + 2];

        idx_t gap1 = p_next - p_cur;
        idx_t gap2 = p_next2 - p_next;

        if (gap1 > gap2) {
            ensure_cache(qual_len + 1);
            qual_cache[qual_len++] = (seq_t)p_cur;
        }
        test_prime_idx++;
    }

    return qual_cache[n - 1];
}
