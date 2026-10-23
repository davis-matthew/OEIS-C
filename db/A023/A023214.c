/*
 * A023214  Primes p such that 4*p + 5 is also prime.
 * https://oeis.org/A023214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 677, u32 n <= 216, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that 4p+5 is also prime (OEIS A023214), found via primality tests. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"

static seq_t *seq_cache = NULL;
static idx_t  cache_len = 0;
static idx_t  cache_cap = 0;
static idx_t  last_checked = 1;

/* Grow the cache array to at least the requested capacity. */
static void grow_cache(idx_t min_cap) {
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Ensure that the cache contains at least n terms of the sequence. */
static void ensure_cache(idx_t n) {
    if (cache_len < n) {
        if (cache_cap < n) grow_cache(n);
        while (cache_len < n) {
            ++last_checked;
            /* test whether last_checked is a prime number */
            if (!is_prime(last_checked)) continue;
            /* compute m = 4*p + 5 and test it for primality */
            seq_t m = ((seq_t)last_checked) * 4 + 5;
            if (!is_prime_big(m)) continue;
            /* store the valid prime p */
            seq_cache[cache_len++] = (seq_t)last_checked;
        }
    }
}

/* External linkage function required by the build system. */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* safety for 1‑offset indices */
    ensure_cache(n);
    return seq_cache[n - 1];
}
