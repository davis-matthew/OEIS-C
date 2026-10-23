/*
 * A038603  Primes not containing the digit '1'.
 * https://oeis.org/A038603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 2983, u32 n <= 8995, u64 n <= 8995,
 *            u128 n <= 8995, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A038603: primes that do not contain the digit '1'.
   a(n) = the n-th prime whose base‑10 representation has no digit 1.
   Offset 1. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h" /* provides bool is_prime(idx_t) */

/* check whether a value contains the digit 1 */
static inline bool contains_digit_one(seq_t x) {
    while (x > 0) {
        if ((x % 10) == 1) return true;
        x /= 10;
    }
    return false;
}

/* cache of sequence values */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t candidate = 2;    /* smallest prime to examine */

/* enlarge the cache if needed */
static void grow_cache(size_t min_cap) {
    if (cache_cap >= min_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!p) abort();
    cache = p;
    cache_cap = new_cap;
}

/* ensure that at least n terms are stored in the cache */
static void ensure_n_terms(idx_t n) {
    if (cache_len >= (size_t)n) return;
    grow_cache((size_t)n);
    while (cache_len < (size_t)n) {
        /* advance candidate until a prime without digit 1 is found */
        for (;;) {
            if (candidate < 2) candidate = 2;            /* guard for wrap‑around */
            if (!contains_digit_one((seq_t)candidate) && is_prime(candidate)) {
                cache[cache_len++] = (seq_t)candidate;
                candidate++;                            /* start searching after this number */
                break;
            }
            candidate++;
        }
    }
}

seq_t a(idx_t n) {
    /* sequence is defined for n >= 1; behaviour for smaller indices is irrelevant */
    if (n == 0) abort();
    ensure_n_terms(n);
    return cache[n - 1];
}
