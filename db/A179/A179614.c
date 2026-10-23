/*
 * A179614  Composite numbers not divisible by any triangular number other
 *          than 1.
 * https://oeis.org/A179614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: composite numbers not divisible by any triangular number > 1
   (OEIS A179614). This implementation searches integers sequentially,
   testing compositeness and divisibility by all triangular numbers > 1. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Simple primality test for numbers that fit in uint64_t. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

/* Static cache of terms discovered so far. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static uint64_t current  = 2;   /* first integer to examine */

/* Return the nth term (1‑based index). */
seq_t a(idx_t n)
{
    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n) {
        uint64_t cand = current;

        /* Identify composite candidates. */
        if (cand > 1 && !is_prime_uint64(cand)) {
            bool ok = true;

            /* Test divisibility by all triangular numbers > 1. */
            for (uint64_t k = 2;; ++k) {
                uint64_t tri = k * (k + 1) / 2;   /* T_k */
                if (tri > cand) break;
                if (cand % tri == 0) {
                    ok = false;
                    break;
                }
            }

            if (ok) {
                /* Ensure there is room for the new value. */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)cand;
            }
        }
        ++current;
    }

    return cache[n - 1];
}
