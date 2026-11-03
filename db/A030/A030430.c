/*
 * A030430  Primes of the form 10*n+1.
 * https://oeis.org/A030430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime of the form 10*k+1
 * Range:     u8 n <= 13, u16 n <= 1625, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 67 terms through n = 8982 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute the A030430 sequence: primes 10*n+1, indexed from 1.   */
/*  Uses lazy caching; the sequence is generated on demand.      */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

static seq_t *cache = NULL;     /* dynamically growing array of primes */
static size_t cache_len = 0;    /* number of primes stored */
static size_t cache_cap = 0;    /* allocated capacity of cache */
static uint64_t next_m = 1;     /* next multiplier; candidate = 10*next_m+1 */

/* Append a new prime into the cache, growing the buffer if needed. */
static void append_prime(seq_t prime) {
    if (cache_len + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();          /* allocation failure – abort */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = prime;
}

/* Return the nth prime of the form 10*k+1.  n is 1‑based.      */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    while (cache_len < n) {
        /* Candidate of the form 10*next_m + 1   */
        uint64_t cand64 = 10ULL * next_m + 1ULL;
        /* Convert to seq_t – safe for the tested range of n (≤10 000) */
        seq_t cand = (seq_t)cand64;
        if (isPrimeGeneric(cand)) {
            append_prime(cand);
        }
        ++next_m;
    }
    return cache[n - 1];
}
