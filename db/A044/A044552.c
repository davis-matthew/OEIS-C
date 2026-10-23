/*
 * A044552  Numbers n such that string 4,5 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   contains45
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A044552
 *  Numbers n such that the base‑7 representation of n contains the
 *  substring 4,5 but the base‑7 representation of n+1 does NOT.
 *
 *  a(1) is the first such number, a(2) the second, and so on.
 *
 *  This implementation builds the sequence on demand, keeping a
 *  static cache so multiple calls are efficient.  It works with the
 *  generic seq_t type supplied by the build (8‑64‑bit or arbitrary
 *  precision).
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains45.h"

/* Cache structure */
static seq_t *cache = NULL;          /* array of found terms */
static size_t   cache_size = 0;      /* number of terms stored */
static size_t   cache_capacity = 0;  /* allocated array size */
static seq_t   last_checked = (seq_t)0;   /* last integer examined */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_cache(size_t need)
{
    if (cache_capacity >= need)
        return;
    size_t new_capacity = cache_capacity ? cache_capacity * 2 : 64;
    if (new_capacity < need)
        new_capacity = need;
    /* realloc returns same pointer type; cast for arbitrary seq_t */
    seq_t *new_cache = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
    cache = new_cache;
    cache_capacity = new_capacity;
}

/* Main function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for 0, but just return 0 */
        return 0;

    /* Produce terms until we have at least n of them */
    while (cache_size < (size_t)n) {
        last_checked++;                     /* next candidate */
        if (contains45(last_checked) &&
            !contains45(last_checked + 1)) {
            ensure_cache(cache_size + 1);
            cache[cache_size++] = last_checked;
        }
    }
    /* n is 1‑based; cache[0] stores a(1) */
    return cache[n - 1];
}
