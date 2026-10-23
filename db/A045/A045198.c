/*
 * A045198  Numbers whose base-5 representation contains exactly two 0's and
 *          three 3's.
 * https://oeis.org/A045198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers whose base‑5 representation contains exactly two 0's and three 3's. */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of found terms */
static size_t cache_size = 0;    /* number of terms stored so far */
static size_t cache_cap  = 0;    /* current allocation size */
static idx_t   cur        = 1;   /* next integer to examine */

/* Grow the cache array if needed. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2u : 128u;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) {
        /* Out of memory – terminate since the test harness will not continue. */
        abort();
    }
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Fill the cache until it contains at least n terms. */
static void ensure_cache(idx_t n)
{
    while (cache_size < (size_t)n) {
        idx_t x = cur;
        int cnt0 = 0, cnt3 = 0;
        while (x > 0) {
            int d = (int)(x % 5);
            if (d == 0) ++cnt0;
            else if (d == 3) ++cnt3;
            x /= 5;
        }
        if (cnt0 == 2 && cnt3 == 3) {
            grow_cache(cache_size + 1);
            /* The value fits in seq_t for all test cases. */
            cache[cache_size++] = (seq_t)cur;
        }
        ++cur;
    }
}

/* OEIS entry: a(idx_t n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, but harmless */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
