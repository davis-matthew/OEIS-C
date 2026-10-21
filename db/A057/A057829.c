/*
 * A057829  Numbers n such that n | 5^n + 3^n +1.
 * https://oeis.org/A057829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   uint64_pow_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A057829: numbers n such that n divides 5^n + 3^n + 1 */

/* We compute terms by scanning natural numbers indefinitely.
 * For each candidate n we test the divisibility condition using a
 * fast modular exponentiation that works with 64‑bit arithmetic
 * (unsigned __int128 is used to avoid overflow).  The results are
 * cached so that subsequent calls to a() with arbitrary indices are
 * answered in O(1) time after the needed terms have been generated. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/uint64_pow_mod.h"

seq_t a(idx_t n)
{
    /* Static cache holding the sequence values that have been found
     * so far.  cache[0] corresponds to a(1). */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;

    /* Next candidate to test.  Since the function may be called
     * in any order, we keep track of the smallest number we have
     * not yet tested. */
    static idx_t next_candidate = 1;

    /* Allocate initial cache space. */
    if (!cache) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed */
    }

    /* Generate terms until we have at least n terms. */
    while (cache_size < n) {
        idx_t cand = next_candidate;
        uint64_t mod = cand;

        /* Compute (5^cand + 3^cand + 1) % cand. */
        uint64_t part5 = uint64_pow_mod(5, cand, mod);
        uint64_t part3 = uint64_pow_mod(3, cand, mod);

        /* Use unsigned __int128 for the addition to prevent overflow. */
        unsigned __int128 sum128 = (unsigned __int128)part5 + part3 + 1;
        uint64_t sum_mod = (uint64_t)(sum128 % mod);

        if (sum_mod == 0) {   /* cand satisfies the condition */
            if (cache_size == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) return 0; /* realloc failed */
                cache = tmp;
            }
            cache[cache_size++] = (seq_t)cand; /* cast may truncate if seq_t smaller */
        }

        ++next_candidate;
    }

    return cache[n - 1];
}
