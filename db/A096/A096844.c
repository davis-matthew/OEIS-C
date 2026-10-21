/*
 * A096844  Numbers where 0 is the only even decimal digit.
 * https://oeis.org/A096844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the nth non‑negative integer whose decimal digits are
 *            chosen from {0,1,3,5,7,9} and that contains at least one 0
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static const int firstDigits[5] = {1, 3, 5, 7, 9};
static const int tailDigits[6]  = {0, 1, 3, 5, 7, 9};

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;
static size_t next_length = 1;

/* Grow the cache to at least new_cap elements. */
static void grow_cache(size_t new_cap) {
    if (new_cap <= cache_cap) return;
    size_t cap = cache_cap ? cache_cap * 2 : 16;
    while (cap < new_cap) cap *= 2;
    seq_t *newptr = (seq_t*)malloc(cap * sizeof(seq_t));
    if (cache) {
        memcpy(newptr, cache, cache_size * sizeof(seq_t));
        free(cache);
    }
    cache = newptr;
    cache_cap = cap;
}

/* Recursively generate numbers of length L, starting from position pos
   (0 ≤ pos < L).  val holds the value built so far.  zeroSeen indicates
   whether a zero digit has appeared. */
static void rec_gen(int pos, int L, seq_t val, bool zeroSeen) {
    if (pos == L) {
        if (zeroSeen) {
            if (cache_size == cache_cap) grow_cache(cache_size + 1);
            cache[cache_size++] = val;
        }
        return;
    }
    for (int i = 0; i < 6; ++i) {
        int d = tailDigits[i];
        bool ns = zeroSeen || d == 0;
        rec_gen(pos + 1, L, val * 10 + (seq_t)d, ns);
    }
}

/* Generate all admissible numbers of length L and add them to the cache. */
static void generate_length(size_t L) {
    if (L == 1) {
        /* Only 0 meets the requirement for 1‑digit numbers. */
        if (cache_size == cache_cap) grow_cache(cache_size + 1);
        cache[cache_size++] = (seq_t)0;
        return;
    }
    /* First digit cannot be 0. */
    for (int i = 0; i < 5; ++i) {
        int fd = firstDigits[i];
        rec_gen(1, (int)L, (seq_t)fd, false);
    }
}

/* Ensure that the cache has at least n entries. */
static void ensure_size(idx_t n) {
    while (cache_size < (size_t)n) {
        generate_length(next_length);
        ++next_length;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* Safety: offset starts at 1. */
    ensure_size(n);
    return cache[n - 1];
}
