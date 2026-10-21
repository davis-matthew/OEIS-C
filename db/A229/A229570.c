/*
 * A229570  Primes of form p*q + 30, where p and q are consecutive primes.
 * https://oeis.org/A229570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 22, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, next_prime, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes of the form p*q + 30, where p and q are consecutive primes (OEIS A229570). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static uint64_t curr_p = 0;
static uint64_t curr_q = 0;
static bool initialized = false;

static void init_sequence(void) {
    if (initialized)
        return;
    curr_p = 2;
    curr_q = next_prime(curr_p);
    initialized = true;
}

static void ensure_cache_capacity(size_t need) {
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;
    if (cache == NULL) {
        cache = seq_alloc(new_cap);
    } else {
        cache = seq_realloc(cache, cache_cap, new_cap);
    }
    if (cache == NULL)
        abort(); /* memory allocation failed */
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0)            /* guard for 0‑based callers */
        return (seq_t)0;

    if (!initialized) {
        init_sequence();
        cache = seq_alloc(16);
        cache_cap = 16;
        cache_len = 0;
    }

    if ((idx_t)cache_len >= n)
        return cache[n - 1];

    while ((idx_t)cache_len < n) {
        seq_t cand = (seq_t)curr_p * (seq_t)curr_q + (seq_t)30;
        if (is_prime_big(cand)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = cand;
        }
        curr_p = curr_q;
        curr_q = next_prime(curr_q);
    }

    return cache[n - 1];
}
