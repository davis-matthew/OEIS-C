/*
 * A158806  Numbers n such that 30*n + 19 is prime.
 * https://oeis.org/A158806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 116, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 81 terms through n = 968 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate terms by testing numbers m such that 30*m+19 is prime */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Cache of the sequence values (the integer m). */
static seq_t *seq_cache = NULL;
static size_t   seq_len  = 0;   /* number of values currently stored */
static size_t   seq_cap  = 0;   /* current allocated capacity */

/* Next candidate m to test.  We start from 0. */
static idx_t last_candidate = 0;

/* Ensure the cache has room for at least need elements. */
static void ensure_capacity(size_t need)
{
    if (seq_cap < need) {
        grow_seq(&seq_cache, &seq_cap, need);
    }
}

/* Return the nth term a(n) for n >= 1. */
seq_t a(idx_t n)
{
    /* Extend the cache until we have at least n terms. */
    while (seq_len < (size_t)n) {
        /* Test the next integer candidate m. */
        idx_t cand = last_candidate++;
        /* Compute 30*cand + 19 in a type large enough to hold the value. */
        uint64_t val = 30ULL * (uint64_t)cand + 19ULL;
        /* Test primality using the provided function. */
        if (is_prime((idx_t)val)) {
            /* Store the candidate in the cache. */
            ensure_capacity(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)cand;
        }
    }
    /* seq_cache[0] corresponds to a(1). */
    return seq_cache[n - 1];
}
