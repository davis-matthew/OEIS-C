/*
 * A228201  Smallest of 11 consecutive primes whose sum is not a prime.
 * https://oeis.org/A228201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 600, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the smallest prime in each block of 11 consecutive primes
   whose sum is composite.  The sequence is generated lazily and
   memoised so that calls for arbitrary n (not necessarily monotonic)
   are fast.  Only integer arithmetic is used; all values fit in seq_t. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

extern idx_t get_prime(idx_t k);  /* not used – kept to satisfy compiler */

static void ensure_cap(size_t needed, size_t *cap, void **ptr, size_t elem_size) {
    if (needed <= *cap) return;
    size_t new_cap = *cap ? *cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    *ptr = realloc(*ptr, new_cap * elem_size);
    *cap = new_cap;
}

/* Very small prime test (suitable for numbers up to about 2·10⁶). */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d <= n / d; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

/* Cached data shared by all calls. */
static seq_t *primes   = NULL;   /* list of primes found so far */
static size_t primes_cap = 0, primes_cnt = 0;
static seq_t *cache    = NULL;   /* sequence values gathered so far */
static size_t cache_cap = 0, cache_len = 0;
static seq_t sum = 0;            /* running sum of the last 11 primes */
static uint64_t cand = 2;        /* next candidate to test for primality */

/* ------------------------------------------------------------ */
/*  Public function required by the build.                       */
/* ------------------------------------------------------------ */
seq_t a(idx_t n) {
    /* n >= 1, index is 1‑based.  Memoisation guarantees
       that previous results are reused when possible. */
    while (cache_len < (size_t)n) {
        /* Find next prime */
        while (!is_prime_uint64(cand)) cand++;
        seq_t p = (seq_t)cand;

        /* Store the new prime */
        ensure_cap(primes_cnt + 1, &primes_cap, (void **)&primes, sizeof(seq_t));
        primes[primes_cnt++] = p;

        /* Update sliding sum of 11 primes */
        sum += p;
        if (primes_cnt > 11) {
            sum -= primes[primes_cnt - 12];
        }

        /* When a full window of 11 primes exists, test the sum */
        if (primes_cnt >= 11) {
            if (!is_prime_uint64((uint64_t)sum)) {
                seq_t start_prime = primes[primes_cnt - 11];
                ensure_cap(cache_len + 1, &cache_cap, (void **)&cache, sizeof(seq_t));
                cache[cache_len++] = start_prime;
            }
        }

        cand++;  /* next integer to test */
    }

    return cache[(size_t)n - 1];
}
