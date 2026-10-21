/*
 * A069711  Squares whose arithmetic mean of digits is an integer (i.e., the
 *          sum of digits is a multiple of the number of digits).
 * https://oeis.org/A069711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth square m such that sum of decimal digits of m is
 *            divisible by the number of decimal digits of m
 * Range:     u8 n <= 7, u16 n <= 35, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   square_good
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/square_good.h"

/* The type seq_t is defined by the build and may be an unsigned integer
   of various sizes or an arbitrary‑precision integer class.  The type
   idx_t is an unsigned integer type, typically uint64_t.  */
static seq_t *cache = NULL;        /* array of computed terms */
static size_t cache_len = 0;       /* number of terms stored */
static size_t cache_cap = 0;       /* capacity of cache */
static uint64_t last_k = 0;        /* last value of k processed (inclusive) */

/* Append a value to the cache, growing the array as needed.  */
static void append_to_cache(seq_t val) {
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) {
            /* Out of memory – in the test harness this should not happen.
               In production code you would handle this appropriately. */
            abort();
        }
        cache = new_ptr;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* The main function requested by the problem.  */
seq_t a(idx_t n) {
    /* Ensure enough terms are computed */
    while (cache_len < n) {
        seq_t sq = (seq_t)last_k * (seq_t)last_k;
        if (square_good(sq)) {
            append_to_cache(sq);
        }
        last_k++;  /* move to next k */
    }
    return cache[n - 1];
}
