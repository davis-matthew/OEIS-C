/*
 * A087915  Even numbers k such that 2*k+3 is a prime.
 * https://oeis.org/A087915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Even numbers k such that 2*k+3 is a prime. */
/* a(n) returns the nth such even k, offset 1. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache      = NULL;   /* array of computed terms */
static size_t cache_len  = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* current capacity */
static idx_t  next_k     = 0;      /* next even k to test */

/* Grow the cache when it is full. */
static void grow_cache_if_needed(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* Fill the cache until it contains at least `target` terms. */
static void ensure_cache(idx_t target)
{
    if (cache_cap == 0) {           /* first allocation */
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    while (cache_len < target) {
        seq_t prime_candidate = (seq_t)2 * next_k + 3;
        if (is_prime_big(prime_candidate)) {
            grow_cache_if_needed();
            cache[cache_len++] = (seq_t)next_k;
        }
        next_k += 2;   /* only even k are considered */
    }
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n<1, safe fallback */
    ensure_cache(n);
    return cache[n - 1];
}
