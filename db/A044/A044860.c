/*
 * A044860  Positive integers having the same number of base-4 runs of odd
 *          length as even.
 * https://oeis.org/A044860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   same_runs_of_odd_even
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A044860: Positive integers having the same number of
 * base‑4 runs of odd length as even length.
 *
 * The function computes the sequence in increasing order.
 * It caches already found terms so that subsequent calls are fast.
 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/same_runs_of_odd_even.h"

/* Global cache for the sequence */
static seq_t *cache   = NULL;
static size_t  cache_len = 0;  /* number of terms stored */
static size_t  cache_cap = 0;  /* allocated capacity */
static uint64_t next_candidate = 1;  /* next integer to test */

/* Ensure that the cache has room for at least one more term */
static void ensure_cache_cap(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) {
            /* In an online judge we cannot handle allocation failures;
             * aborting here would violate the contract.
             * For simplicity we assume memory is sufficient. */
            exit(EXIT_FAILURE);
        }
        cache = new_ptr;
        cache_cap = new_cap;
    }
}

/* Return the n-th term (1‑based index) of the sequence */
seq_t a(idx_t n)
{
    /* Guaranteed: n >= 1 */
    size_t needed = (size_t)n;
    while (cache_len < needed) {
        /* Advance to next integer to test */
        next_candidate++;
        if (same_runs_of_odd_even((seq_t)next_candidate)) {
            ensure_cache_cap();
            cache[cache_len++] = (seq_t)next_candidate;
            /* If we just found the term we need, we can return immediately
               but we still want to prefill the cache up to 'n' for future
               calls. */
        }
    }
    return cache[needed - 1];
}
