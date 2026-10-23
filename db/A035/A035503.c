/*
 * A035503  Upper of pair of consecutive happy numbers.
 * https://oeis.org/A035503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 1607, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 6449, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the upper of a pair of consecutive happy numbers.
   The sequence consists of all numbers N such that N-1 and N are
   both happy numbers (i.e. repeatedly replacing the number by the
   sum of the squares of its digits eventually reaches 1).
   The function keeps a cache of found upper numbers so that arbitrary
   calls to a(n) are O(1) after the required terms have been computed. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Determines whether a 64‑bit integer is happy.  The usual
   algorithm that stops when the sequence reaches 1 (happy)
   or 4 (entering the known non‑happy cycle). */
static bool is_happy_uint64(uint64_t n)
{
    while (true) {
        if (n == 1) return true;
        if (n == 4) return false;   /* 4 eventually loops to 16 → 37 → 58 → 89 → 145 → 42 → 20 → 4 */
        uint64_t sum = 0;
        while (n > 0) {
            uint64_t d = n % 10;
            sum += d * d;
            n /= 10;
        }
        n = sum;
    }
}

/* The required public function. */
seq_t a(idx_t n)
{
    /* Static state: cache of found upper numbers and the next integer
       to test for a consecutive happy pair. */
    static seq_t *cache = NULL;     /* array of found upper numbers (i+1) */
    static size_t cache_len = 0;    /* number of elements stored */
    static size_t cache_cap = 0;    /* allocated capacity of cache */
    static idx_t next_i = 2;        /* the next integer to test as the lower of a pair */

    /* Grow the cache when necessary. */
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed; abort to avoid undefined behaviour */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        idx_t i = next_i;
        if (is_happy_uint64((uint64_t)i) && is_happy_uint64((uint64_t)(i + 1))) {
            cache[cache_len++] = (seq_t)(i + 1); /* store the upper of the pair */
        }
        next_i = i + 1; /* advance to the next integer */
    }

    return cache[n - 1];
}
