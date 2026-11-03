/*
 * A179003  Numbers k such that 2^(2*k-1) mod (2*k+1) is prime.
 * https://oeis.org/A179003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   is_prime, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A179003: numbers k such that
   2^(2*k-1) mod (2*k+1) is prime. The function searches k
   sequentially and memoises the results so that calls in any
   order are efficient. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/is_prime.h"
#include "../utils/powmod.h"
#include "../utils/mem.h"

static seq_t *cache = NULL;     /* memoised list of k */
static size_t cache_size = 0;   /* number of stored k */
static size_t cache_cap = 0;    /* current capacity of cache */
static idx_t next_k = 1;        /* next k to test */

/* Ensure cache has at least 'need' slots. */
static void grow_cache(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? (cache_cap << 1) : 8;
        while (new_cap < need)
            new_cap <<= 1;
        seq_t *newp = mem_resize(seq_t, cache, new_cap);
        cache = newp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Lazily compute terms until we have at least n of them. */
    while (cache_size < (size_t)n) {
        /* Compute m = 2*k+1 and exponent e = 2*k-1. */
        idx_t k = next_k;
        seq_t m   = (seq_t)(k * 2 + 1);
        uint64_t e = (uint64_t)k * 2 - 1;

        /* Calculate r = 2^e mod m. */
        seq_t r = powmod((seq_t)2, e, m);

        /* If the residue r is prime, record k. */
        if (is_prime((idx_t)r)) {
            grow_cache(cache_size + 1);
            cache[cache_size++] = (seq_t)k;
        }
        ++next_k;
    }
    return cache[(size_t)n - 1];
}
