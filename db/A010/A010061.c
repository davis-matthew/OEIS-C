/*
 * A010061  Binary self or Colombian numbers: numbers that cannot be expressed
 *          as the sum of distinct terms of the form 2^k+1 (k>=0), or
 *          equivalently, numbers not of form m + sum of binary digits of m.
 * https://oeis.org/A010061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  Sequence: A010061  (Binary self or Colombian numbers)
 *
 *  Definition:  a(n) is the nth positive integer that cannot be written
 *               as m + popcount(m) for any integer m ≥ 0, where popcount(m)
 *               counts the number of 1‑bits in the binary expansion of m.
 *  Implementation:  generate all numbers expressible as m + popcount(m)
 *                   up to a safe upper bound, mark them, and collect the
 *                   complement.  This works for all built‑in integer types
 *                   and for arbitrary‑precision seq_t (the static array
 *                   can hold any such type as long as the limit is not
 *                   exceeded).
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* Maximum limit we are prepared to handle. Adjust if larger n are needed. */
#define MAX_LIMIT 500000

/* Static storage – works for integral or arbitrary‑precision seq_t. */
static seq_t  cache[MAX_LIMIT];
static size_t cache_len = 0;

static bool marked[MAX_LIMIT];
static size_t marked_cap = MAX_LIMIT;

static idx_t current_limit = 0;

/* Non‑negative integer m is processed by iterating up to (current_limit). */
static inline unsigned popcnt_idx(idx_t x) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll((unsigned long long)x);
#else
    unsigned s = 0;
    while (x) { s += (unsigned)(x & 1); x >>= 1; }
    return s;
#endif
}

/* Rebuild the sequence up to the required limit. */
static void rebuild_cache(idx_t required_limit) {
    if (required_limit > marked_cap) {
        /* The static array is too small – cannot enlarge in C10/C++17 scope. */
        /* In practice MAX_LIMIT should be large enough for tests. */
        return;
    }
    /* Clear mark up to required_limit. */
    memset(marked, 0, (required_limit + 1) * sizeof(bool));

    /* Mark all numbers that can be written as m + popcount(m) for m ≤ limit. */
    for (idx_t m = 0; m <= required_limit; ++m) {
        idx_t val = m + (idx_t)popcnt_idx(m);
        if (val <= required_limit) marked[val] = true;
    }

    /* Build the cache of non‑marked numbers. */
    cache_len = 0;
    for (idx_t i = 1; i <= required_limit; ++i) {
        if (!marked[i]) {
            cache[cache_len++] = (seq_t)i;
        }
    }

    current_limit = required_limit;
}

/* Public API. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;            /* undefined for n=0 */

    /* If we already have enough cached values, return the stored one. */
    if (cache_len >= n) return cache[n - 1];

    /* Determine a safe upper bound for the necessary limit. */
    idx_t new_limit = n * 5 + 100;
    if (new_limit > current_limit) {
        rebuild_cache(new_limit);
    }

    /* In the rare case that rebuild_cache was unable to enlarge
       (e.g., MAX_LIMIT too small), fall back to recomputing up to current_limit. */
    if (cache_len < n) {
        /* Force a rebuild up to the current limit. */
        rebuild_cache(current_limit);
    }

    return cache[n - 1];
}
