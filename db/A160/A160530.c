/*
 * A160530  Positive integers that contain only odd-length runs of 0's and 1's
 *          in their binary expansion.
 * https://oeis.org/A160530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 2583, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A160530 (OEIS A160530) by sequentially testing integers.
   A(n) is the nth positive integer whose binary expansion contains
   only odd‑length runs of 0’s and 1’s. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/bitlen_seq_t.h"

/* Return true if all runs of consecutive 0’s or 1’s in x have odd length. */
static bool odd_runs(seq_t x)
{
    /* x > 0 is guaranteed by construction */
    size_t bits = bitlen_seq_t(x);
    if (bits <= 1)
        return true;

    bool last_bit = ((x >> (bits - 1)) & 1) != 0;
    size_t run_len = 1;

    for (int64_t i = (int64_t)bits - 2; i >= 0; --i) {
        bool bit = ((x >> (unsigned long long)i) & 1) != 0;
        if (bit == last_bit) {
            ++run_len;
        } else {
            if ((run_len & 1) == 0)
                return false;
            last_bit = bit;
            run_len = 1;
        }
    }
    return (run_len & 1) != 0;
}

/* Return the nth term of sequence A160530. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t candidate = (seq_t)1;
    static bool initialized = false;

    if (!initialized) {
        cache_cap = 128;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        cache_len = 0;
        candidate = (seq_t)1;
        initialized = true;
    }

    /* Extend cache until it contains at least n terms. */
    while ((idx_t)cache_len < n) {
        /* Find next candidate that satisfies the run condition. */
        while (!odd_runs(candidate)) {
            candidate = candidate + ((seq_t)1);
        }

        /* Grow cache if necessary. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) abort();      /* allocation failure */
            cache = new_cache;
            cache_cap = new_cap;
        }

        cache[cache_len++] = candidate;
        candidate = candidate + ((seq_t)1);
    }

    return cache[n - 1];
}
