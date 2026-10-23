/*
 * A039108  Numbers whose base-9 representation has the same number of 5's and
 *          8's.
 * https://oeis.org/A039108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n-th non‑negative integer whose base‑9
 *            representation * contains the same number of 5’s and 8’s. * The
 *            function searches numbers in ascending order and caches the
 *            results * so that calls with arbitrary n are fast
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_equal_5_and_8, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"   /* seq_alloc()  */
#include "../utils/seq_realloc.h"/* seq_realloc() */
#include "../utils/has_equal_5_and_8.h"

/* static cache of generated terms */
static seq_t *cache    = NULL;   /* array of a(1), a(2), ...          */
static size_t cache_len = 0;     /* number of elements filled          */
static size_t cache_cap = 0;     /* allocated capacity                 */
static seq_t next_candidate  = 0;/* next number to test                 */

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1; undefined for n < 1 */
        return 0;

    /* lazy initialization of cache */
    if (cache_cap == 0) {
        cache_cap = 32;
        cache = seq_alloc(cache_cap);
        if (!cache) abort();
    }

    /* generate terms until we have n of them */
    while (cache_len < (size_t)n) {
        if (has_equal_5_and_8(next_candidate)) {
            if (cache_len == cache_cap) {          /* grow cache   */
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) abort();                /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
