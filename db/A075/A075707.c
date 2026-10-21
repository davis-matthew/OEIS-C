/*
 * A075707  Safe primes (A005385) (p and (p-1)/2 are primes) such that 12*p+1
 *          is also prime.
 * https://oeis.org/A075707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 22, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A075707: safe primes p (p and (p-1)/2 prime) such that 12*p+1 is prime. */
/* The function returns the n-th such prime for any n ≥ 1. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */

/* ---------- cache of discovered sequence elements ---------- */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static void ensure_cache(size_t newlen) {
    if (newlen <= cache_cap) return;
    size_t newcap = cache_cap ? cache_cap * 2 : 64;
    while (newcap < newlen) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = newcap;
}
static void cache_append(seq_t val) {
    ensure_cache(cache_len + 1);
    cache[cache_len++] = val;
}

/* ---------- generator of sequence terms ---------- */
static seq_t next_candidate = 5;   /* first odd ≥5 */
static bool initialized = false;

static void generate_terms(idx_t n) {
    if (!initialized) { next_candidate = 5; initialized = true; }
    while (cache_len < (size_t)n) {
        /* test candidate for primality */
        if (is_prime_big(next_candidate)) {
            /* (p-1)/2 must be prime */
            seq_t half = (next_candidate - 1) / 2;
            if (is_prime_big(half)) {
                /* 12*p+1 must be prime */
                seq_t mp = 12 * next_candidate + 1;
                if (is_prime_big(mp))
                    cache_append(next_candidate);
            }
        }
        next_candidate += 2;   /* skip even numbers */
    }
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n) {
    if (n < 1) return 0;
    generate_terms(n);
    return cache[n - 1];
}
