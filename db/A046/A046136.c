/*
 * A046136  Primes p such that p, p+4 and p+10 are primes.
 * https://oeis.org/A046136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 307, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime p such that p, p+4, and p+10 are all prime. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 3;

/* Return the n‑th term of A046136.  Calls to a() may appear in any order.  */
seq_t a(idx_t n) {
    while (cache_len < n) {
        if (is_prime(next_candidate) &&
            is_prime(next_candidate + 4) &&
            is_prime(next_candidate + 10)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2U : 64U;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                assert(tmp);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
