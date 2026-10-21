/*
 * A201017  Primes whose multiplicative digital root is 5.
 * https://oeis.org/A201017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   digit_product, is_prime_big, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes whose multiplicative digital root is 5
   a(n) = the n-th prime p with mdroot(p) = 5, n >= 1   (a(1) = 5) */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/is_prime_big.h"
#include "../utils/digit_product.h"
#include "../utils/seq_alloc.h"

/* static cache of qualifying primes */
static seq_t *prime_cache = NULL;
static size_t cache_len   = 0;   /* number of primes stored   */
static size_t cache_cap   = 0;   /* allocated capacity        */
static seq_t next_candidate = 2; /* next integer to test      */

/* Expand the cache when necessary */
static void grow_cache(void) {
    if (cache_len >= cache_cap) {
        size_t new_cap = (cache_cap == 0 ? 64 : cache_cap * 2);
        seq_t *new_buf = seq_alloc(new_cap);
        for (size_t i = 0; i < cache_len; ++i) {
            new_buf[i] = prime_cache[i];
        }
        prime_cache = new_buf;
        cache_cap   = new_cap;
    }
}

/* Multiplicative digital root */
static seq_t mdroot(seq_t v) {
    while (v >= 10) {
        v = digit_product(v);
    }
    return v;
}

/* Main function required by the build system */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined, safe fallback */

    /* Search until we have the first n qualifying primes */
    while (cache_len < (size_t)n) {
        /* Test the current candidate */
        if (is_prime_big(next_candidate) &&
            mdroot(next_candidate) == 5) {
            grow_cache();
            prime_cache[cache_len++] = next_candidate;
        }
        /* Move to the next integer */
        next_candidate = next_candidate + 1;
    }
    return prime_cache[n - 1];
}
