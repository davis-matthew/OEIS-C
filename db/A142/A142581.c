/*
 * A142581  Primes congruent to 51 mod 53.
 * https://oeis.org/A142581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime congruent to 51 (mod 53)
 * Range:     u8 n <= 1, u16 n <= 122, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is memoised and works for any build of seq_t and idx_t. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- helpers ---------- */

/* Integer square‑root (floor) for 64‑bit values */
static uint64_t isqrt64(uint64_t n)
{
    if (n < 2) return n;
    uint64_t x = n, y = (x >> 1) + 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* Prime storage: dynamic array of 64‑bit primes */
static uint64_t *primes = NULL;
static size_t   primes_cap = 0;
static size_t   primes_len = 0;

/* Ensure that `primes` contains all primes <= `limit`. */
static void ensure_primes_up_to(uint64_t limit)
{
    if (limit <= (primes_len ? primes[primes_len - 1] : 1))
        return;

    uint64_t start = (primes_len ? primes[primes_len - 1] + 1 : 2);
    for (uint64_t cand = start; cand <= limit; ++cand) {
        if ((cand & 1ULL) == 0 && cand != 2)
            continue;                     /* skip even numbers */

        bool is_prime = true;
        uint64_t sq = isqrt64(cand);
        for (size_t i = 0; i < primes_len && primes[i] <= sq; ++i) {
            if (cand % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (primes_len == primes_cap) {
                primes_cap = primes_cap > 0 ? primes_cap * 2 : 16;
                uint64_t *tmp = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!tmp) abort();
                primes = tmp;
            }
            primes[primes_len++] = cand;
        }
    }
}

/* Test if a 64‑bit integer is prime using the current prime list. */
static bool is_prime_candidate(uint64_t cand)
{
    uint64_t lim = isqrt64(cand);
    ensure_primes_up_to(lim);
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if (p > cand / p) break;
        if (cand % p == 0) return false;
    }
    return true;
}

/* ---------- main function ---------- */

seq_t a(idx_t n)
{
    /* Memoisation storage. */
    static seq_t *cache = NULL;
    static size_t  cache_cap = 0;
    static size_t  cache_len = 0;
    static uint64_t next_candidate = 3U;   /* smallest odd > 2 that ≡ 51 (mod 53) */

    if (n == 0) return 0; /* offset is 1 */

    /* Grow cache until we have at least n entries. */
    while (cache_len < (size_t)n) {
        /* Find the next prime ≡ 51 (mod 53). */
        for (;;) {
            /* Ensure we are looking at an odd number. */
            if ((next_candidate & 1ULL) == 0)
                ++next_candidate;
            if (next_candidate % 53 != 51) {
                next_candidate++;
                continue;
            }
            if (is_prime_candidate(next_candidate))
                break;
            next_candidate++;
        }

        /* Store the found prime into the cache. */
        if (cache_len == cache_cap) {
            cache_cap = cache_cap > 0 ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
        }
        cache[cache_len++] = (seq_t)next_candidate;

        /* Move to the next integer for the following search. */
        ++next_candidate;
    }

    return cache[(size_t)n - 1];
}
