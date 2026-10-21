/*
 * A069822  Numbers k for which there is at least one x < k such that sigma(x)
 *          = sigma(k).
 * https://oeis.org/A069822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   numbers k such that sigma(k) has appeared before
 * Range:     u8 n <= 38, u16 n <= 9679, u32 n <= 9679, u64 n <= 9679,
 *            u128 n <= 9679, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a hash table to remember previously seen sigma
   values and an increasing list of all k that have a duplicate sigma.
   It works in C11 and C++17 alike. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"

#define HASH_SIZE (1u << 18)   /* 262144, ample for the required range */

static uint64_t *hash_table = NULL;
static bool hash_initialized = false;

static void init_hash(void)
{
    hash_table = (uint64_t *)malloc(HASH_SIZE * sizeof(uint64_t));
    if (!hash_table) abort();
    for (size_t i = 0; i < HASH_SIZE; ++i) hash_table[i] = 0;
    hash_initialized = true;
}

/* Return true if s has already been seen, false otherwise.
   Insert s if it is new. */
static bool seen_or_insert(uint64_t s)
{
    if (!hash_initialized) init_hash();
    size_t idx = s & (HASH_SIZE - 1);
    while (true) {
        uint64_t v = hash_table[idx];
        if (v == 0) {          /* empty slot */
            hash_table[idx] = s;
            return false;      /* seen for the first time */
        }
        if (v == s)            /* duplicate encountered */
            return true;
        idx = (idx + 1) & (HASH_SIZE - 1);   /* linear probing */
    }
}

/* Dynamic array of results */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t max_k_processed = 0;

static void ensure_cache_capacity(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    while (cache_len < (size_t)n) {
        ++max_k_processed;
        seq_t sigma = sigma_of(max_k_processed);
        uint64_t sig = (uint64_t)sigma;          /* sigma fits in 64 bits for k up to ~2·10⁵ */
        if (seen_or_insert(sig)) {
            /* k has a duplicate sigma, add to cache */
            ensure_cache_capacity();
            cache[cache_len++] = (seq_t)max_k_processed;
        }
    }
    return cache[(size_t)n - 1];
}
