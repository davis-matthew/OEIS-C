/*
 * A211485  Numbers for which the canonical prime factorization contains only
 *          an odd number of exponents, all of which are congruent to 1 modulo
 *          3.
 * https://oeis.org/A211485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose canonical prime factorization contains only an
   odd number of exponents, all of which are congruent to 1 modulo 3. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/smallest_prime_factor.h"

static seq_t *cache      = NULL;  /* stored terms */
static size_t cache_len  = 0;     /* number of stored terms */
static size_t cache_cap  = 0;     /* allocated capacity */
static uint64_t last_checked = 1; /* last integer examined */

/* Test whether n satisfies the condition. */
static bool is_good_uint64(uint64_t n) {
    if (n == 1) return true;          /* 1 is included by convention */

    uint64_t temp = n;
    size_t distinct = 0;

    while (temp > 1) {
        uint64_t p = smallest_prime_factor(temp);
        if (p == 0) return false;     /* safety, should not occur */

        size_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }

        if (exp % 3U != 1U) return false; /* exponent not 1 mod 3 */
        ++distinct;
    }

    return (distinct & 1U) != 0;       /* odd number of distinct primes */
}

/* Grow the cache until it contains at least n terms. */
static void grow_cache_to(idx_t n) {
    if (cache_len >= (size_t)n) return;

    /* Allocate initial cache if necessary. */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return;              /* allocation failure */
        cache[0] = (seq_t)1;              /* first term */
        cache_len = 1;
        last_checked = 1;
    }

    while (cache_len < (size_t)n) {
        ++last_checked;                   /* examine next integer */

        if (is_good_uint64(last_checked)) {
            /* Resize cache if needed. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return;          /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }
}

/* Public function required by the build. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined, but return 0 */
    grow_cache_to(n);
    return cache[n - 1];
}
