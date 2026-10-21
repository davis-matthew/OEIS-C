/*
 * A064962  Numbers k such that 1000k+1, 1000k+3, 1000k+7, 1000k+9 are all
 *          primes.
 * https://oeis.org/A064962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 23, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A064962
   Numbers k such that 1000k+1, 1000k+3, 1000k+7, 1000k+9 are all primes.
   Computed by brute‑force search with is_prime_u64. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;
    static idx_t  next_k = 1;   /* candidate k to test next */

    /* Generate more values until we have at least n entries. */
    while (cache_size < n) {
        uint64_t k  = (uint64_t)next_k;          /* work in 64‑bit for the test */
        uint64_t v1 = 1000ULL * k + 1ULL;
        if (!is_prime_u64(v1)) { next_k++; continue; }

        uint64_t v3 = v1 + 2ULL;
        if (!is_prime_u64(v3)) { next_k++; continue; }

        uint64_t v7 = v1 + 6ULL;
        if (!is_prime_u64(v7)) { next_k++; continue; }

        uint64_t v9 = v1 + 8ULL;
        if (!is_prime_u64(v9)) { next_k++; continue; }

        /* All four numbers are prime: store k. */
        if (cache_size >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failure – abort in caller */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_size++] = (seq_t)k;
        next_k = k + 1;
    }

    return cache[n - 1];   /* 1‑based OEIS offset */
}
