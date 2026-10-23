/*
 * A039528  Numbers whose base-12 representation has the same nonzero number
 *          of 3's and 9's.
 * https://oeis.org/A039528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth integer whose base‑12 representation has the same *
 *            non‑zero number of digits 3 and 9, found by brute‑force search
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* static state for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t last_checked = 0;

/* append value to cache, growing array as needed */
static void push_to_cache(seq_t val) {
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1024;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) abort();           /* out of memory */
        cache = new_ptr;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* test whether a number satisfies the 3/9 rule in base 12 */
static bool good_seq(seq_t x) {
    seq_t t = x;
    unsigned c3 = 0, c9 = 0;
    while (t != (seq_t)0) {
        unsigned d = (unsigned)(t % (seq_t)12);
        if (d == 3) c3++;
        else if (d == 9) c9++;
        t /= (seq_t)12;
    }
    return c3 > 0 && c3 == c9;
}

/* public entry point requested by the exercise */
seq_t a(idx_t n) {
    while (cache_len < n) {
        last_checked++;
        if (good_seq((seq_t)last_checked))
            push_to_cache((seq_t)last_checked);
    }
    return cache[n - 1];
}
