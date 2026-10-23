/*
 * A045409  Primes congruent to {3, 4, 5} mod 7.
 * https://oeis.org/A045409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime that is congruent to 3, 4, or 5 modulo 7
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static uint64_t *prime_cache = NULL;   /* stored qualifying primes      */
static size_t  prime_cache_size = 0;   /* number of stored primes       */
static size_t  prime_cache_cap  = 0;   /* allocated capacity of cache   */
static uint64_t last_checked = 2;      /* next candidate to test       */

static void ensure_cache_capacity(size_t needed)
{
    if (needed > prime_cache_cap) {
        size_t new_cap = prime_cache_cap ? prime_cache_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        void *tmp = realloc(prime_cache, new_cap * sizeof(uint64_t));
        if (!tmp) abort();               /* unrecoverable allocation failure */
        prime_cache = (uint64_t*)tmp;
        prime_cache_cap = new_cap;
    }
}

/* Append a new qualifying prime to the cache */
static void add_prime(uint64_t p)
{
    ensure_cache_capacity(prime_cache_size + 1);
    prime_cache[prime_cache_size++] = p;
    last_checked = p + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* below OEIS offset */
    while (prime_cache_size < n) {
        for (uint64_t cand = last_checked; ; ++cand) {
            if (!is_prime(cand)) continue;
            uint64_t r = cand % 7;
            if (r == 3 || r == 4 || r == 5) {
                add_prime(cand);
                break;
            }
        }
    }
    return (seq_t)prime_cache[n - 1];
}
