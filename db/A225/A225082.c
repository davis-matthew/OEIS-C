/*
 * A225082  Centrally deletable primes.
 * https://oeis.org/A225082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 2406, u32 n <= 1465, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Distributed computation of OEIS A225082 (centrally deletable primes).
   The function incrementally finds primes, tests central‑digit deletion,
   and memoises results so calls in any order are fast. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* Helper: test a 64‑bit integer for primality using the supplied big‑int routine. */
static inline bool is_prime_uint64(uint64_t x)
{
    return is_prime_big((seq_t)x);
}

/* Return true if prime p (as uint64_t) stays prime after deleting its central digit(s). */
static bool is_centrally_deletable(uint64_t p)
{
    if (p < 10)          /* single‑digit primes cannot be deleted */
        return false;

    /* Extract digits into a buffer (least significant first). */
    uint8_t rev[32];
    int d = 0;
    uint64_t t = p;
    while (t > 0 && d < 32) {
        rev[d++] = (uint8_t)(t % 10);
        t /= 10;
    }
    if (d < 2)           /* need at least two digits */
        return false;

    /* Digits in most‑significant order. */
    uint8_t dig[32];
    for (int i = 0; i < d; ++i)
        dig[i] = rev[d - 1 - i];

    /* Indices of digits to skip. */
    int skip1 = -1, skip2 = -1;
    if (d & 1) {                    /* odd length – delete one middle digit */
        skip1 = d / 2;
    } else {                        /* even length – delete two middle digits */
        skip1 = d / 2 - 1;
        skip2 = d / 2;
    }

    /* Reconstruct the number without the middle digit(s). */
    uint64_t res = 0;
    for (int i = 0; i < d; ++i) {
        if (i == skip1 || i == skip2)
            continue;
        res = res * 10 + dig[i];
    }

    if (res == 0)            /* deletion removed all digits (e.g. 2‑digit primes) */
        return false;
    return is_prime_uint64(res);
}

/* --- memoised sequence construction --- */
static seq_t *cache = NULL;           /* array of cached terms */
static size_t cache_len = 0;          /* number of terms stored so far */
static uint64_t next_candidate = 2;   /* next integer to test for primality */

/* Grow cache until it contains at least n terms. */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        /* Skip non‑primes */
        while (!is_prime_uint64(next_candidate))
            ++next_candidate;

        /* If the prime is centrally deletable, store it. */
        if (is_centrally_deletable(next_candidate)) {
            seq_t *tmp = (seq_t *)realloc(cache, (cache_len + 1) * sizeof(seq_t));
            if (!tmp) abort();  /* allocation failure */
            cache = tmp;
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1; return 0 for safety */
        return 0;
    ensure_cache(n);
    return cache[n - 1];
}
