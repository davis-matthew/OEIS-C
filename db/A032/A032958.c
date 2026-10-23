/*
 * A032958  Numbers whose base-8 representation Sum_{i=0..m} d(i)*8^(m-i) has
 *          even d(i) for all odd i.
 * https://oeis.org/A032958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 131, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   valid_oct_evens
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032958: numbers whose base‑8 representation has even
   digits at all odd positions (counting from the most significant digit).
   The function generates the sequence incrementally, caching results
   to support nondeterministic order of calls. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/valid_oct_evens.h"

/* Cache for previously computed sequence values. */
static seq_t *cache = NULL;
static idx_t cached_cnt = 0;     /* number of terms currently cached */
static size_t cache_cap = 0;     /* capacity of the cache */

static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                 /* unrecoverable allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Next candidate value to test after the last value examined. */
static seq_t next_candidate = 1;

/* Main function requested by the interface. */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined; return 0 for safety */
        return (seq_t)0;

    /* Make sure the cache has at least n slots. */
    ensure_cache_capacity((size_t)n);

    /* Generate new terms until we have at least n of them. */
    while (cached_cnt < n) {
        if (valid_oct_evens(next_candidate))
            cache[cached_cnt++] = next_candidate;
        ++next_candidate;   /* advance candidate regardless of validity */
    }

    return cache[n - 1];
}
