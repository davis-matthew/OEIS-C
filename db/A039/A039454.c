/*
 * A039454  Numbers whose base-11 representation has the same nonzero number
 *          of 1's and 8's.
 * https://oeis.org/A039454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, changed
 * Formula:   a(n) = the n-th positive integer whose base‑11 representation
 *            contains the same non‑zero number of 1's and 8's
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 65 terms through n = 969 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* check whether a number has equal non‑zero numbers of 1s and 8s in base 11 */
static bool has_equal_1_8(seq_t x) {
    int cnt1 = 0;
    int cnt8 = 0;
    while (x > 0) {
        int d = (int)(x % 11);
        if (d == 1) cnt1++;
        else if (d == 8) cnt8++;
        x = x / 11;
    }
    return (cnt1 == cnt8 && cnt1 > 0);
}

static seq_t *cache = NULL;     /* cache of found sequence values */
static size_t cache_len = 0;    /* number of values stored */
static size_t cache_cap = 0;    /* current capacity of cache */
static seq_t cur = 1;           /* next candidate to test */

/* grow the cache if needed */
static void ensure_capacity(size_t needed) {
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) {
        /* allocation failed – terminate the program */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    size_t target = (size_t)n;
    if (target == 0) return 0;  /* offset is 1, but guard against bad input */

    while (cache_len < target) {
        if (has_equal_1_8(cur)) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = cur;
        }
        cur += 1;
    }
    return cache[target - 1];
}
