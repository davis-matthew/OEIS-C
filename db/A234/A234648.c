/*
 * A234648  Even sums of 2 consecutive odious numbers (A000069).
 * https://oeis.org/A234648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 5461, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A234648: even sums of two consecutive odious numbers. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static state for memoization */
static seq_t *a_cache = NULL;
static size_t a_cache_len = 0;
static size_t a_cache_cap = 0;
static idx_t last_number = 0;
static idx_t prev_odious = 0;
static bool have_prev = false;

/* Test whether a 64‑bit unsigned integer is odious (odd popcount). */
static inline bool is_odious(idx_t x)
{
    return (__builtin_popcountll((unsigned long long)x) & 1);
}

/* Grow the cache array to at least `needed` elements. */
static void grow_cache(size_t needed)
{
    if (needed <= a_cache_cap)
        return;
    size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_mem = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cache = new_mem;
    a_cache_cap = new_cap;
}

/* Main entry point.  Generates terms on demand and memoizes them. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* offset is 1; 0 is never requested under odds */

    /* Generate as many terms as required */
    while (a_cache_len < n) {
        last_number++;
        if (is_odious(last_number)) {
            if (have_prev) {
                seq_t sum = (seq_t)prev_odious + (seq_t)last_number;
                if ((sum & 1) == 0) {
                    grow_cache(a_cache_len + 1);
                    a_cache[a_cache_len++] = sum;
                }
            }
            prev_odious = last_number;
            have_prev   = true;
        }
    }
    return a_cache[n - 1];
}
