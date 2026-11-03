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
 * Validated: standard, pass, 72 terms through n = 4959 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true if the sequence type value "x" contains at least one
   decimal digit equal to 7.  Uses only % and / by 10 to inspect digits. */
static bool contains7(seq_t x)
{
    while (x != 0)
    {
        if ((x % 10) == 7) return true;
        x /= 10;
    }
    return false;
}

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
    seq_t *p = mem_resize(seq_t, cache, new_size);
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
