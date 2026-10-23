/*
 * A039073  Numbers whose base-8 representation has the same number of 4's and
 *          7's.
 * https://oeis.org/A039073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑8 representation has the same number of 4's and 7's.
   Computed by a linear search.  The sequence is monotone increasing, so the
   nth term a(n) is simply the nth integer (starting from 0) that satisfies the
   digit‑count condition.  The function builds a cache of these numbers so
   that repeated calls with arbitrary n are efficient. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;
static uint64_t checked_upto = 0;

/* Ensure that cache contains at least n values, expanding if necessary. */
static void ensure_cache(idx_t n)
{
    /* initialise cache on first use */
    if (cache == NULL) {
        size_t cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return;             /* out of memory – let caller handle */
        cache_cap = cap;
        /* 0 is always a valid member (no 4's and no 7's) */
        cache[0] = (seq_t)0;
        cache_size = 1;
        checked_upto = 1;
    }

    /* grow cache capacity if needed */
    if (n > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;        /* out of memory – let caller handle */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* extend sequence until we have at least n entries */
    while (cache_size < (size_t)n) {
        uint64_t x = checked_upto;
        int diff = 0;
        if (x == 0) { /* already added earlier, skip */
        } else {
            uint64_t y = x;
            while (y) {
                unsigned d = y % 8;
                if (d == 4) diff++;
                else if (d == 7) diff--;
                y /= 8;
            }
            if (diff == 0) {
                cache[cache_size++] = (seq_t)x;
            }
        }
        checked_upto = x + 1;
    }
}

/* External interface required by the specification */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* outside offset – behaviour unspecified */
    ensure_cache(n);
    /* a is 1‑indexed; cache[0] is a(1) */
    return cache[n - 1];
}
