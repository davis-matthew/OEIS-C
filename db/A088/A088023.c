/*
 * A088023  Set a(1) = 1. Then take the list of defined initial terms, reverse
 *          their order, add 1, 2, 3, ... to the reversed list in succession
 *          and append this new list to the right of the previously defined
 *          terms. Repeat this process indefinitely.
 * https://oeis.org/A088023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 506, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   highest_power_of_two_le
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A088023
 *
 * a(1) = 1,      a(2) = 2
 * For n ≥ 3:
 *   Let t = 2^⌊log₂ n⌋  (the greatest power of two ≤ n)
 *   If n = t and t ≥ 4,     a(n) = 2·a(n/2) – 1
 *   else (n > t),             a(n) = a(2t – n + 1) + (n – t)
 *
 * This implementation uses memoised recursion and a dynamically
 * growing cache of seq_t values.  The cache index 0 is unused
 * (the sequence starts at index 1).  seq_t and idx_t are defined
 * in the build environment.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/highest_power_of_two_le.h"

static seq_t *cache = NULL;      /* cached a(n) values */
static size_t cache_cap = 0;     /* allocated elements (indices 0 .. cache_cap-1) */

/* Forward declaration of the recursive helper. */
static seq_t a_recursive(idx_t n);

/* --------------------------------------------------------------------- */
seq_t a(idx_t n) {               /* public entry point */
    return a_recursive(n);
}

/* --------------------------------------------------------------------- */
/* Cache handling: grow the cache to contain indices up to 'n'. */
static void init_cache(idx_t n) {
    if (cache_cap > (size_t)n) return;          /* already large enough */

    size_t new_cap = 2 * ((size_t)n + 1);        /* double to keep growth log‑linear */

#ifdef __cplusplus
    /* Allocate new array, copy old values, then delete old. */
    seq_t *tmp = new seq_t[new_cap];
    for (size_t i = 0; i < cache_cap; ++i) tmp[i] = cache[i];
    delete [] cache;
    cache = tmp;
#else
    seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;                           /* allocation failed – leave cache unchanged */
    /* Zero the newly allocated part so that default values are 0. */
    memset(tmp + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    cache = tmp;
#endif
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* Recursive implementation with memoisation. */
static seq_t a_recursive(idx_t n) {
    if (n == 1) return (seq_t)1;              /* base case */

    init_cache(n);
    if (cache[n] != 0) return cache[n];      /* cached value (sequence is positive) */

    idx_t t = highest_power_of_two_le(n);
    seq_t result;

    if (n == t) {                            /* n is a power of two */
        if (t == 2) {
            result = (seq_t)2;
        } else {
            result = a_recursive(t >> 1) * 2 - 1;
        }
    } else {                                 /* n > t */
        /* m = 2t – n + 1  ∈ [2, t]  – always smaller than n. */
        idx_t m = (t << 1) - n + 1;
        result = a_recursive(m) + (seq_t)(n - t);
    }

    cache[n] = result;
    return result;
}
