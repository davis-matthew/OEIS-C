/*
 * A175758  Numbers n with property that arithmetic mean of its digits is both
 *          an integer and one of the prime factors of n.
 * https://oeis.org/A175758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers n such that the arithmetic mean of its decimal digits is
 *            an integer and a prime factor of n
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_count, digit_sum, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Digit utilities and small‑prime test */
#include "../utils/digit_sum.h"     /* seq_t digit_sum(seq_t x); */
#include "../utils/digit_count.h"   /* uint64_t digit_count(uint64_t n); */
#include "../utils/is_prime_small.h"/* bool is_prime_small(seq_t n); */

#ifdef __cplusplus
/* C++ build: use std::vector for dynamic storage of seq_t objects */
static std::vector<seq_t> cache_vec;
#elif defined(__STDC__)
// C build: use plain malloc/realloc on POD types
static seq_t *cache = NULL;
static size_t  cache_len = 0;
static size_t  cache_cap = 0;
#endif

/* Common state for both builds */
static uint64_t tested = 1;                /* next integer to test */

/* --------------------------------------------------------------------- */
static void ensure_cache(idx_t needed)
{
#if defined(__cplusplus)
    /* Expand the vector until it contains at least 'needed' elements. */
    while (cache_vec.size() < (size_t)needed) {
        uint64_t m = tested++;
        seq_t sum = digit_sum((seq_t)m);
        uint64_t len = digit_count(m);
        if (sum % (seq_t)len != 0) continue;                  /* mean not integer */
        seq_t mean = sum / (seq_t)len;
        if (mean <= 1 || !is_prime_small(mean)) continue;     /* must be prime */
        if ((uint64_t)m % (uint64_t)mean != 0) continue;      /* not a divisor  */
        cache_vec.push_back((seq_t)m);
    }
#else
    /* C build: grow raw array as needed. */
    while (cache_len < (size_t)needed) {
        /* Grow the allocation if necessary. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { /* allocation failed; leave cache unchanged */ }
            else {
                cache     = tmp;
                cache_cap = new_cap;
            }
        }

        uint64_t m = tested++;
        seq_t sum   = digit_sum((seq_t)m);
        uint64_t len = digit_count(m);
        if (sum % (seq_t)len != 0) continue;                  /* mean not integer */
        seq_t mean = sum / (seq_t)len;
        if (mean <= 1 || !is_prime_small(mean)) continue;     /* must be prime */
        if ((uint64_t)m % (uint64_t)mean != 0) continue;      /* not a divisor  */

        /* Store the found term. */
        cache[cache_len++] = (seq_t)m;
    }
#endif
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* undefined, but avoid undefined behaviour */
    ensure_cache(n);          /* guarantee that the first n terms are cached */

#if defined(__cplusplus)
    return cache_vec[(size_t)n - 1];
#else
    return cache[(size_t)n - 1];
#endif
}
