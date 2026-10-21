/*
 * A062902  Number and its reversal are both multiples of 12.
 * https://oeis.org/A062902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 452, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Searching for 12‑multiples whose decimal reversal is also a multiple of 12.
    a(n) is the nth such integer, with a(1)=0.  The function keeps a cache of
    previously found terms and scans successive multiples of 12 until the
    requested index is available.  It performs only a few integer operations
    per candidate, making it easily fast enough for n up to 10 000. */

#include <stdlib.h>

/* reverse of an integer in decimal */
static seq_t reverse(seq_t x) {
    seq_t r = 0;
    while (x > 0) {          /* while x has digits */
        r = r * 10 + (x % 10);
        x = x / 10;
    }
    return r;
}

/* static cache of results */
static seq_t *cache = NULL;    /* array of found terms */
static size_t cache_len = 0;   /* number of terms stored */
static size_t cache_cap = 0;   /* capacity of the array */
static seq_t next_val = 0;     /* next multiple of 12 to test */

/* enlarge the cache when necessary */
static void add_term(seq_t v) {
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = v;
}

/* public function */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* safety for non‑positive indices */
    /* return cached value if already computed */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /* generate terms until we have enough */
    while (cache_len < (size_t)n) {
        /* test current candidate; all candidates are multiples of 12 by construction */
        if (reverse(next_val) % (seq_t)12 == 0) {
            add_term(next_val);
        }
        next_val += (seq_t)12;   /* move to next multiple of 12 */
    }
    return cache[(size_t)n - 1];
}
