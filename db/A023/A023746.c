/*
 * A023746  Plaindromes: numbers whose digits in base 4 are in nondecreasing
 *          order.
 * https://oeis.org/A023746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 165, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Plaindromes: numbers whose base‑4 digits are in nondecreasing order. */
/* Values are generated in ascending order. */
/* The function caches results, so any order of calls is supported. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* static state for memoisation */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_len = 1;    /* length of numbers to generate next */
static bool initialized = false;

/* Ensure cache has at least needed capacity */
static void ensure_cap(size_t needed) {
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp) {
            cache = tmp;
            cache_cap = new_cap;
        }
    }
}

/* Add a new value to the cache */
static void push_cache(seq_t val) {
    ensure_cap(cache_len + 1);
    cache[cache_len++] = val;
}

/* Recursive generator for numbers of a given length */
static void gen_len_rec(idx_t pos, idx_t len, seq_t val, uint8_t last) {
    if (pos == len) {
        push_cache(val);
        return;
    }
    for (uint8_t d = last; d <= 3; ++d) {
        seq_t new_val = val * (seq_t)4 + (seq_t)d;
        gen_len_rec(pos + 1, len, new_val, d);
    }
}

/* Generate all numbers of a particular length len */
static void gen_len(idx_t len) {
    for (uint8_t d = 1; d <= 3; ++d) {
        gen_len_rec(1, len, (seq_t)d, d);
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* behaviour below offset is irrelevant */

    /* Initialise cache lazily */
    if (!initialized) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache_len = 0;
        push_cache((seq_t)0);             /* a(1) = 0 */
        initialized = true;
    }

    /* Return cached value if already computed */
    if (n <= (idx_t)cache_len) return cache[n - 1];

    /* Generate further terms until we reach desired index */
    while (cache_len < (size_t)n) {
        gen_len(next_len);
        ++next_len;
    }

    return cache[n - 1];
}
