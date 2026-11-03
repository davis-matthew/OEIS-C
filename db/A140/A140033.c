/*
 * A140033  Primes of the form 8x^2+8xy+233y^2.
 * https://oeis.org/A140033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 212, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 56 terms through n = 9958 (2026-11-03)
 * Helpers:   mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes the sequence A140033: primes of the form 8x^2 + 8xy + 233y^2.
   a(n) is the n‑th prime satisfying this quadratic representation (offset 1).
   The function is memoised and works for any call order.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stddef.h>
#include "../utils/next_prime.h"
#include "../utils/mem.h"

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

/* Test whether a prime p can be written as 8x^2+8xy+233y^2. */
static bool represents_prime(uint64_t p)
{
    /* product 32*p fits 64 bits for all p that appear in the sequence
       (the first 10 000 terms are far below 2^63). */
    uint64_t prod_p = 32ULL * p;
    /* maximum y such that 7392*y^2 ≤ 32*p   */
    uint64_t y_max = 0;
    if (prod_p > 7392ULL)
        y_max = (uint64_t)floor(sqrt((double)prod_p / 7392.0));
    for (uint64_t y = 0; y <= y_max; ++y) {
        uint64_t ysq     = y * y;
        uint64_t d       = prod_p - 7392ULL * ysq;          /* D = 32p – 7392y² ≥ 0   */
        uint64_t t       = (uint64_t)sqrt((double)d);      /* floor sqrt(D)        */
        if (t * t != d)          continue;                  /* not a perfect square  */
        if (t % 8 != 0)          continue;                  /* sqrt must be multiple of 8 */
        uint64_t k        = t / 8;
        if (((k - y) & 1ULL) == 0) return true;            /* parity condition      */
    }
    return false;
}

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t last_checked = 1; /* the last prime that was examined */

/* Grow the cache to accommodate at least new_len elements. */
static void grow_cache(size_t new_len)
{
    if (new_len <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < new_len) new_cap *= 2;
    seq_t *new_cache = seq_alloc_new_array(new_cap);
    if (!new_cache) abort();
    /* copy existing values (the objects are already constructed) */
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];
    /* free the old buffer (objects were trivially destructible) */
    mem_free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; safe return for 0 */
    /* Ensure we have computed at least the first n terms */
    while (cache_len < n) {
        uint64_t p = next_prime(last_checked + 1);
        last_checked = p;
        if (represents_prime(p)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = p;
        }
    }
    return cache[n - 1];
}
