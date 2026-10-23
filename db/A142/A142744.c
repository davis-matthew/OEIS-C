/*
 * A142744  Primes congruent to 17 mod 59.
 * https://oeis.org/A142744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 109, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142744: primes congruent to 17 (mod 59). */
/* a(n) = the n-th prime p such that p ≡ 17 (mod 59), with offset 1. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Append a value to the dynamic cache, growing its capacity as needed. */
static void append_to_cache(seq_t **cache, size_t *len, size_t *cap, seq_t val)
{
    if (*cap == 0) {
        *cap = 4;
        *cache = (seq_t *)malloc(*cap * sizeof(seq_t));
    } else if (*len == *cap) {
        size_t new_cap = *cap * 2;
        *cache = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
    (*cache)[*len] = val;
    (*len)++;
}

seq_t a(idx_t n)
{
    /* The function must work for any n ≥ 1. */
    if (n == 0) return 0; /* behaviour for index 0 is irrelevant */

    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 17; /* first number ≡ 17 (mod 59) */

    /* Generate primes until we have at least n terms cached. */
    while (cache_len < n) {
        if (is_prime(next_candidate)) {
            append_to_cache(&cache, &cache_len, &cache_cap,
                            (seq_t)next_candidate);
        }
        next_candidate += 59;
    }
    return cache[n - 1];
}
