/*
 * A137059  Numbers k such that k and k^2 use only the digits 1, 4, 8 and 9.
 * https://oeis.org/A137059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137059: Numbers k such that k and k² use only the digits 1,4,8,9.
   a(n) = the n‑th such number (offset 1). */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* seq_t and idx_t are provided by the build */
static seq_t *cache = NULL;      /* dynamic array of found values */
static size_t   cache_len = 0;   /* number of elements stored   */
static size_t   cache_cap = 0;   /* allocated capacity          */

/* keep track of the number of digits that have been fully processed */
static idx_t cur_len = 1;

/* Ensure the dynamic array can hold at least needed elements */
static void ensure_cap(size_t needed) {
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Append a new value to the array */
static void append(seq_t v) {
    ensure_cap(cache_len + 1);
    cache[cache_len++] = v;
}

/* Test whether all decimal digits of x belong to {1,4,8,9} */
static bool allowed_digits(seq_t x) {
    if (x == 0) return false;          /* 0 is never allowed */
    while (x != 0) {
        unsigned d = (unsigned)(x % (seq_t)10);
        if (d != 1 && d != 4 && d != 8 && d != 9) return false;
        x /= (seq_t)10;
    }
    return true;
}

/* Search all numbers with exactly len decimal digits
   whose digits are in the allowed set. */
static void generate_len_rec(int len, int pos, seq_t val) {
    static const int digs[4] = {1, 4, 8, 9};
    if (pos == len) {
        if (allowed_digits(val) && allowed_digits(val * val)) {
            append(val);
        }
        return;
    }
    for (int i = 0; i < 4; ++i) {
        generate_len_rec(len, pos + 1, val * 10 + digs[i]);
    }
}

/* Make sure we have at least n terms in the cache */
static void ensure_n_terms(idx_t n) {
    while (cache_len < (size_t)n) {
        generate_len_rec((int)cur_len, 0, (seq_t)0);
        cur_len++;
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    ensure_n_terms(n);
    return cache[(size_t)n - 1];
}
