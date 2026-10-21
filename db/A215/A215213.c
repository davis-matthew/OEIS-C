/*
 * A215213  Primes congruent to {2, 5, 7} mod 19.
 * https://oeis.org/A215213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 2, 5, or 7 modulo 19, offset
 *            1
 * Range:     u8 n <= 12, u16 n <= 826, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, residue_index_from_mod19
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm keeps a cache of already found primes.  When a new n is
   requested we continue generating candidates of the form
   19*k + r with r in {2,5,7}, test them for primality with
   is_prime_big(), and append the ones that succeed to the cache. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t) */
#include "../utils/residue_index_from_mod19.h"

static seq_t *cache      = NULL;  /* array of found primes */
static size_t cache_cap  = 0;      /* current allocation */
static size_t cache_len  = 0;      /* number of primes stored */

static const unsigned incs[3] = {3, 2, 14}; /* 2→5 (+3), 5→7 (+2), 7→2+19 (+14) */

/* Ensure that the cache contains at least n primes. */
static void ensure_cached(idx_t n) {
    if (n <= cache_len) return;           /* already have enough */

    /* Grow the allocation if necessary. */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    seq_t last_candidate;
    int pre_index;                 /* index into incs for next increment */

    if (cache_len == 0) {
        cache[0] = (seq_t)2U;      /* first prime in the sequence */
        cache_len = 1;
        last_candidate = 2U;
        pre_index = 0;             /* residue index for 2 */
    } else {
        last_candidate = cache[cache_len - 1];
        pre_index = residue_index_from_mod19(last_candidate);
        /* The next candidate will use increments[pre_index]. */
    }

    while (cache_len < n) {
        seq_t candidate = last_candidate + (seq_t)incs[pre_index];
        pre_index = (pre_index + 1) % 3;
        last_candidate = candidate;
        if (is_prime_big(candidate)) {
            cache[cache_len++] = candidate;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* guard for offset 1 */
    ensure_cached(n);
    return cache[n - 1];
}
