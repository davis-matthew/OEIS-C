/*
 * A039426  Numbers whose base-10 representation has the same nonzero number
 *          of 4's and 8's.
 * https://oeis.org/A039426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A039426: numbers whose base‑10 representation
 * contains the same non‑zero number of 4’s and 8’s.  The function
 * memoises results so that calls with arbitrary ordering of n are
 * efficient. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

/* Helper: test whether a non‑negative integer (stored in uint64_t) has
 * an equal, non‑zero number of digits 4 and 8. */
static bool has_equal_4_and_8(uint64_t x)
{
    int cnt4 = 0, cnt8 = 0;
    if (x == 0) return false;
    while (x) {
        unsigned d = (unsigned)(x % 10);
        if (d == 4) ++cnt4;
        else if (d == 8) ++cnt8;
        x /= 10;
    }
    return (cnt4 > 0) && (cnt4 == cnt8);
}

/* seq_t and idx_t are supplied by the build system. */

/* Grow the cache array to at least 'need' elements.  The array holds
 * seq_t values; copy via assignment so that it works for arbitrary‑
 * precision types. */
static void grow_cache(size_t need)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static uint64_t last_checked = 0;   /* last integer tested */

    if (cap < need) {
        size_t new_cap = cap ? cap * 2 : 64;
        while (new_cap < need) new_cap <<= 1;
        seq_t *new_cache = mem_new(seq_t, new_cap);
        if (!new_cache) return; /* allocation failed – should not happen */
        for (size_t i = 0; i < len; ++i) new_cache[i] = cache[i];
        mem_free(cache);
        cache = new_cache;
        cap   = new_cap;
    }

    for (; len < need; ++last_checked) {
        if (has_equal_4_and_8(last_checked)) {
            cache[len++] = (seq_t)last_checked;
            if (len == need) return;
        }
    }
}

/* Main public function implementing the OEIS sequence. */
seq_t a(idx_t n)
{
    /* On first call init static state */
    static int init = 0;
    static seq_t *cache   = NULL;
    static size_t cap     = 0;
    static size_t len     = 0;
    static uint64_t last  = 0;
    if (!init) {
        init = 1;
        cap   = 64;
        cache = mem_new(seq_t, cap);
    }

    /* Grow cache as needed */
    if (n > len) {
        size_t need = (size_t)n;
        if (cap < need) {
            size_t new_cap = cap ? cap * 2 : 64;
            while (new_cap < need) new_cap <<= 1;
            seq_t *new_cache = mem_new(seq_t, new_cap);
            for (size_t i = 0; i < len; ++i) new_cache[i] = cache[i];
            mem_free(cache);
            cache = new_cache;
            cap   = new_cap;
        }
        for (; len < need; ++last) {
            if (has_equal_4_and_8(last)) {
                cache[len++] = (seq_t)last;
            }
        }
    }
    return cache[n-1];
}
