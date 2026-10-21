/*
 * A061379  Difference between n and its reversal is a perfect cube.
 * https://oeis.org/A061379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 431, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_cube_root, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence of numbers k such that k - rev(k) is a perfect cube. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/reverse_digits.h"
#include "../utils/floor_cube_root.h"

/* Determines if x is a perfect cube. */
static bool is_cube(seq_t x)
{
    seq_t c = floor_cube_root(x);       /* integer cube root, floor */
    seq_t prod = c * c * c;
    return prod == x;
}

/* Stores generated terms and keeps state between calls. */
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_count = 0;
static seq_t next_candidate = 1;

static void add_to_cache(seq_t val)
{
    if (cache_count == cache_size) {
        size_t new_size = cache_size ? cache_size * 2 : 256;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        /* Ignore realloc failure – in the contest environment enough memory
           will be available. */
        cache = tmp;
        cache_size = new_size;
    }
    cache[cache_count++] = val;
}

/* Generates terms until at least n terms are stored. */
static void extend_cache(idx_t n)
{
    while (cache_count < n) {
        seq_t k = next_candidate;
        seq_t rev = reverse_digits(k);
        seq_t diff = k > rev ? k - rev : rev - k;
        if (is_cube(diff))
            add_to_cache(k);
        next_candidate++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for offset 1, but safe */
    extend_cache(n);
    return cache[n - 1];
}
