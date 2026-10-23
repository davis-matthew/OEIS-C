/*
 * A145475  Primes p such that (17+p)/2 is prime.
 * https://oeis.org/A145475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth such prime (all terms are congruent to 5 mod 12)
 * Range:     u8 n <= 8, u16 n <= 421, u32 n <= 489, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that (17+p)/2 is prime (OEIS A145475) */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;          /* stored sequence values */
static size_t cache_len = 0;         /* number of values stored */
static size_t cache_cap = 0;         /* allocated size of cache */
static seq_t next_candidate = 2;     /* next integer to test for the property */

/* Append a value to the cache, growing it if necessary */
static void push_cache(seq_t val) {
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_arr) abort();       /* out‑of‑memory */
        cache = new_arr;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Main function requested by the problem statement */
seq_t a(idx_t n) {
    if (n == 0) abort();            /* access is 1‑based, offset=1 */

    /* Generate sequence values until we have at least n terms */
    while (cache_len < n) {
        if (is_prime_big(next_candidate)) {
            seq_t q = (next_candidate + (seq_t)17) / (seq_t)2;
            if (is_prime_big(q)) {
                push_cache(next_candidate);
            }
        }
        ++next_candidate;           /* move to the next integer */
    }

    return cache[n - 1];
}
