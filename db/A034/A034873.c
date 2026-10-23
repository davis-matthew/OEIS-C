/*
 * A034873  Primes in which each digit occurs in runs of at least 2.
 * https://oeis.org/A034873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   is_prime, run_digit_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A034873: primes where each digit occurs in consecutive runs of length at least 2. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/run_digit_ok.h"

static seq_t *cache = NULL;          /* cache of valid primes */
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t next_candidate = 11; /* first candidate prime to test */

/* Ensure that cache contains at least n primes satisfying the digit‑run condition. */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;

    while (cache_len < n) {
        /* Skip non‑primes */
        while (!is_prime(next_candidate)) ++next_candidate;

        seq_t prime = (seq_t)next_candidate;
        if (run_digit_ok(prime)) {
            if (cache_len == cache_cap) {
                size_t newcap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) abort();           /* allocation failure */
                cache = tmp;
                cache_cap = newcap;
            }
            cache[cache_len++] = prime;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    if (n == 0) return (seq_t)0;  /* undefined for n<1 */
    return cache[n - 1];
}
