/*
 * A142751  Primes congruent to 24 mod 59.
 * https://oeis.org/A142751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime congruent to 24 modulo 59
 * Range:     u8 n <= 1, u16 n <= 108, u32 n <= 40, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

static seq_t cache[200000];          /* cache of found primes */
static size_t cache_len = 0;         /* number of primes cached */
static seq_t next_candidate = 0;     /* next number to test */
static bool initialized = false;    /* has init been run? */

static void init_cache(void) {
    if (!initialized) {
        next_candidate = (seq_t)83;  /* first prime ≡ 24 (mod 59) */
        initialized = true;
    }
}

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n) {
    init_cache();

    /* Expand the cache until it contains n primes. */
    while (cache_len < (size_t)n) {
        seq_t cand = next_candidate;
        if (is_prime_big(cand)) {
            cache[cache_len++] = cand;
        }
        next_candidate += (seq_t)59;   /* move to the next candidate in the progression */
    }
    return cache[n - 1];
}
