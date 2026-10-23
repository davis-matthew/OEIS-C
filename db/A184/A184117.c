/*
 * A184117  Lower s-Wythoff sequence, where s(n) = 2n + 1.
 * https://oeis.org/A184117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Lower s-Wythoff sequence with s(n) = 2n + 1
 * Range:     u8 n <= 180, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool */
#include <stdlib.h>   /* malloc, realloc, exit */
#include <stdint.h>   /* uint64_t */

/* Static cache for the sequence terms. */
static seq_t *a_cache = NULL;
static size_t a_cache_size   = 0;  /* number of elements stored */
static size_t a_cache_cap    = 0;  /* allocated capacity */

/* Static boolean array marking numbers that have already appeared as a or b. */
static bool *used = NULL;
static size_t used_cap = 0;  /* allocated capacity (indexes 0 .. used_cap-1) */

/* The next positive integer to test as a candidate.  Starts at 1. */
static uint64_t next_candidate = 1;

/* Ensure that the a_cache array can hold at least 'needed' elements. */
static void ensure_a_capacity(size_t needed)
{
    if (needed > a_cache_cap) {
        size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 16;
        while (new_cap <= needed) new_cap <<= 1;
        seq_t *new_arr = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!new_arr) exit(1);
        a_cache = new_arr;
        a_cache_cap = new_cap;
    }
}

/* Ensure that the used array can hold at least index 'idx'. */
static void ensure_used_capacity(uint64_t idx)
{
    if (idx >= used_cap) {
        size_t new_cap = used_cap ? used_cap * 2 : 16;
        while (new_cap <= idx) new_cap <<= 1;
        bool *new_arr = (bool *)realloc(used, new_cap * sizeof(bool));
        if (!new_arr) exit(1);
        /* Initialise new entries to false. */
        for (size_t i = used_cap; i < new_cap; ++i)
            new_arr[i] = false;
        used = new_arr;
        used_cap = new_cap;
    }
}

/* Main function.  External linkage. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n=0, but safe */

    /* If already computed, just return the stored value. */
    if ((size_t)n <= a_cache_size)
        return a_cache[n - 1];

    /* Compute terms up to n, storing them in the cache. */
    for (idx_t i = (idx_t)a_cache_size + 1; i <= n; ++i) {
        /* Compute s(i) = 2i + 1, using seq_t. */
        seq_t s = ((seq_t)i) * 2 + 1;

        /* Find the least positive integer not yet used. */
        while (1) {
            ensure_used_capacity(next_candidate);
            if (!used[(size_t)next_candidate]) break;
            next_candidate++;
        }

        /* Current candidate is the value for a(i). */
        seq_t a_val = (seq_t)next_candidate;

        /* Compute corresponding b(i) = a(i) + s. */
        seq_t b_val = a_val + s;

        /* Mark both a(i) and b(i) as used. */
        ensure_used_capacity((uint64_t)a_val);
        used[(size_t)a_val] = true;
        ensure_used_capacity((uint64_t)b_val);
        used[(size_t)b_val] = true;

        /* Store a(i) in the cache. */
        ensure_a_capacity(a_cache_size + 1);
        a_cache[a_cache_size++] = a_val;

        /* Advance next_candidate to the next unused number. */
        next_candidate++;
        while (next_candidate < used_cap && used[(size_t)next_candidate])
            next_candidate++;
    }

    return a_cache[n - 1];
}
