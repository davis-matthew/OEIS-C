/*
 * A039098  Numbers whose base-9 representation has the same number of 3's and
 *          5's.
 * https://oeis.org/A039098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 154, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9608 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑9 representation has the same number of 3's and 5's. */
/* This implementation searches successive integers and stores the ones
   that satisfy the condition in a static cache.  On every call it
   extends the cache if necessary so that the sequence can be
   requested in any order.  It works with any seq_t/idx_t types
   permitted by the build. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Count 3's and 5's in base‑9 representation of x. */
static bool base9_equal_3_5(idx_t x)
{
    unsigned c3 = 0, c5 = 0;
    if (x == 0)                   /* 0 has zero 3's and zero 5's */
        return true;
    while (x > 0) {
        unsigned d = (unsigned)(x % 9);
        if (d == 3) ++c3;
        else if (d == 5) ++c5;
        x /= 9;
    }
    return c3 == c5;
}

static seq_t *cache = NULL;      /* cache of computed terms                */
static size_t cache_len = 0;     /* number of terms stored                 */
static size_t cache_cap = 0;     /* allocated capacity                     */
static idx_t next_start = 0;     /* next integer to test                   */

/* Grow cache to accommodate at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap < needed)
        new_cap <<= 1;           /* double until enough space */
    seq_t *newp = mem_resize(seq_t, cache, new_cap);
    cache = newp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)                    /* undefined but safe return */
        return (seq_t)0;

    /* Initialise cache once */
    if (cache == NULL) {
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
        cache[0] = (seq_t)0;       /* a(1) = 0 */
        cache_len = 1;
        next_start = 1;            /* next integer to test */
    }

    /* Extend cache until it contains n terms */
    while (cache_len < n) {
        for (idx_t v = next_start;; ++v) {
            if (base9_equal_3_5(v)) {
                grow_cache(cache_len + 1);
                cache[cache_len] = (seq_t)v;
                ++cache_len;
                next_start = v + 1;
                break;
            }
        }
    }

    return cache[n - 1];
}
