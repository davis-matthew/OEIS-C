/*
 * A142953  Primes congruent to 61 mod 64.
 * https://oeis.org/A142953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 61 mod 64
 * Range:     u8 n <= 1, u16 n <= 210, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 1000 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

static seq_t *cache     = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t  next_k    = 0;

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    }

    /* generate primes until we have at least n of them */
    while (cache_len < n) {
        uint64_t cand = 64ULL * (uint64_t)next_k + 61ULL;
        ++next_k;

        /* skip candidates that no longer fit into seq_t */
        if (cand > (uint64_t)~(seq_t)0) {
            continue;
        }

        /* primality test on the candidate */
        if (is_prime_seq(cand)) {
            /* grow cache if needed */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = (seq_t)cand;
        }
    }

    return cache[n - 1];
}
