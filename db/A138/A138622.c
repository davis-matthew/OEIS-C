/*
 * A138622  Nonnegative integers k such that 14*k-1 is prime.
 * https://oeis.org/A138622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1967, u32 n <= 99, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th nonnegative integer k such that 14*k-1 is prime. */
/* The function works in C11 and in C++17 using a simple incremental algorithm. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

#ifdef __cplusplus
#include <vector>
#endif

/* State kept between calls. */
static idx_t next_k = 0;         /* next candidate k to evaluate */

#ifdef __cplusplus
/* In C++ we use a std::vector for automatic construction/destruction. */
static std::vector<seq_t> cacheVec;
#else
/* In C we use a raw array that we grow manually. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
#endif

/* Grow the C array to hold at least 'needed' elements. */
#ifndef __cplusplus
static void ensure_cache_cap(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}
#endif

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined by OEIS offset – return 0 as a safe value */
        return 0;

    /* Extend the cache until we have at least n entries. */
    while (true) {
#ifdef __cplusplus
        if (cacheVec.size() >= n) break;
#else
        if (cache_len >= n) break;
#endif
        /* Compute the candidate prime test. */
        seq_t candidate = (seq_t)14 * (seq_t)next_k - (seq_t)1;
        /* Check that candidate is a prime; nonnegative k are allowed. */
        if (is_prime_big(candidate)) {
#ifdef __cplusplus
            cacheVec.push_back((seq_t)next_k);
#else
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)next_k;
#endif
        }
        ++next_k;
    }

    /* Return the requested term. */
#ifdef __cplusplus
    return cacheVec[n - 1];
#else
    return cache[n - 1];
#endif
}
