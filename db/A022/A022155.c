/*
 * A022155  Values of n at which Golay-Rudin-Shapiro sequence A020985 is
 *          negative.
 * https://oeis.org/A022155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   runs_parity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by searching indices where the parity of the number of 1‑bits
   differs from the parity of the number of runs of 1‑bits in the binary
   representation.  That is, a(n) is the nth integer m ≥ 0 such that
   popcount(m) % 2 != runs(m) % 2, where runs(m) counts the number of
   maximal consecutive runs of 1s in the binary expansion of m.  The
   sequence equals the positions at which the Golay–Rudin–Shapiro
   sequence A020985 takes the value –1. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/runs_parity.h"

/* Helper: parity of popcount */
static inline int popcount_parity(uint64_t x) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x) & 1;
#else
    /* Portable fallback */
    int p = 0;
    while (x) {
        p ^= 1;
        x &= x - 1;
    }
    return p;
#endif
}

/* Static cache for computed terms */
static seq_t *cache = NULL;          /* array of results */
static size_t cache_cap = 0;         /* capacity of cache */
static idx_t cache_size = 0;         /* number of terms stored */
static uint64_t last_checked = 0;    /* last integer examined */

/* Ensure cache contains at least n terms (n >= 1). */
static void ensure_cache(idx_t n) {
    while (cache_size < n) {
        /* Expand cache if necessary */
        if (cache_size >= cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort(); /* Out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Search next candidate */
        last_checked++; /* start from 0, then 1, 2, ... */
        if (popcount_parity(last_checked) != runs_parity(last_checked)) {
            cache[cache_size++] = (seq_t)last_checked;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* undefined for n==0, but harmless */
    ensure_cache(n);
    return cache[n - 1];
}
