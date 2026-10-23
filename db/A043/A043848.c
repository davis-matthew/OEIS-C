/*
 * A043848  Numbers n such that number of runs in the base 4 representation of
 *          n is congruent to 5 mod 7.
 * https://oeis.org/A043848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer whose number of runs in the base‑4
 *            representation is congruent to 5 modulo 7. The implementation
 *            performs a simple search, caching results so that calls in any
 *            order are efficient
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Helper: count runs in base‑4 representation of x. */
static inline int runs_in_base4(seq_t x)
{
    int runs = 0;
    int prev = -1;
    while (x > 0) {
        int digit = (int)(x & (seq_t)3);
        if (digit != prev) {
            ++runs;
            prev = digit;
        }
        x >>= 2;
    }
    return runs;
}

/* Caching state shared across calls. */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;
static seq_t last_checked = 0;

seq_t a(idx_t n)
{
    /* Grow the cache until it contains at least n entries. */
    while (cache_size < n) {
        seq_t x = last_checked + 1; /* start scanning after the last cached value */
        while (1) {
            if (runs_in_base4(x) % 7 == 5) {
                /* Ensure capacity. */
                if (cache_size == cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 64;
                    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!tmp) {
                        /* Allocation failed – return 0 as a sentinel. */
                        return (seq_t)0;
                    }
                    cache = tmp;
                }
                cache[cache_size++] = x;
                last_checked = x;
                break; /* found one; move to next required term */
            }
            ++x;
        }
    }
    return cache[n - 1];
}
