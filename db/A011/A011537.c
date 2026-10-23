/*
 * A011537  Numbers that contain at least one 7.
 * https://oeis.org/A011537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th positive integer whose decimal representation
 *            contains at least one digit 7
 * Range:     u8 n <= 43, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   contains7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains7.h"

/* Cached sequence values: a(1) stored at cache[0], etc.  */
static seq_t *cache = NULL;
static idx_t cache_size = 0;    /* number of entries allocated */
static idx_t cache_len  = 0;    /* number of values computed (cache[0]..cache[cache_len-1]) */
static seq_t next_search = 0;   /* last integer examined (starts at 0) */

/* Ensure that the cache has at least "need" elements allocated. */
static void ensure_capacity(idx_t need)
{
    if (cache_size >= need) return;
    idx_t new_size = cache_size ? cache_size << 1 : 16;
    if (new_size < need) new_size = need;
    seq_t *p = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!p) abort();
    cache = p;
    cache_size = new_size;
}

/* External function required by the build system.  */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                  /* a(0) is not defined, return 0 safely */
    if (n <= cache_len) return cache[n-1];

    ensure_capacity(n);

    for (idx_t i = cache_len; i < n; ++i)
    {
        seq_t cand = next_search + 1;
        while (!contains7(cand)) ++cand;
        cache[i] = cand;
        next_search = cand;
    }
    cache_len = n;
    return cache[n-1];
}
