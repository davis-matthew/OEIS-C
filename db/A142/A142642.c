/*
 * A142642  Primes congruent to 3 mod 56.
 * https://oeis.org/A142642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 271, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142642: primes congruent to 3 (mod 56) */
/* 
 * a(n) = the nth prime p such that p mod 56 = 3,  n ≥ 1.
 * The implementation uses a simple incremental search.  For each new
 * required term it examines successive integers k ≡ 3 (mod 56) and
 * tests them for primality with the supplied is_prime() routine.
 * Results are cached so that repeated calls for the same or larger
 * indices are answered in O(1) time.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t x) */

static seq_t *cache = NULL;     /* array of primes ≡ 3 mod 56 */
static idx_t  cache_len = 0;    /* number of primes cached */
static idx_t  cache_cap = 0;    /* current allocation size */
static uint64_t last_candidate = 2;   /* last number tested (will start at 3) */

/* Ensure that cache contains at least n primes ≡ 3 (mod 56). */
static void ensure_cached(idx_t n)
{
    if (cache_len >= n) return;        /* enough terms already cached */

    /* Allocate initial buffer if needed. */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    while (cache_len < n) {
        /* Advance last_candidate to the next integer ≡ 3 (mod 56). */
        do {
            last_candidate++;
        } while (last_candidate % 56u != 3u);

        /* Test primality. */
        if (is_prime(last_candidate)) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) abort();
            }
            cache[cache_len++] = (seq_t)last_candidate;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* behaviour below offset is irrelevant */
    ensure_cached(n);
    return cache[n - 1];
}
