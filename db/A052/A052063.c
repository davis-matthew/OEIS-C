/*
 * A052063  Numbers k such that the decimal expansion of k^3 contains no
 *          palindromic substring except single digits.
 * https://oeis.org/A052063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 28, u32 n <= 1394, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cube_has_pal_subseq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A052063: numbers k such that k³ has no palindromic
    decimal substring of length ≥2.  Implementation uses only
    arithmetic operators on the supplied types seq_t and idx_t.
    The sequence is generated on demand; results are cached
    to speed up repeated calls.  */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/cube_has_pal_subseq.h"

/* --------- dynamic cache for previously found terms --------- */
static seq_t *cache = NULL;
static size_t cache_len   = 0;
static size_t cache_cap   = 0;
static seq_t next_k = 0;           /* next integer to test */

/* grow the cache to at least new_cap elements */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    size_t newsize = new_cap * sizeof(seq_t);
    seq_t *newbuf = (seq_t*)malloc(newsize);
    if (newbuf == NULL) exit(EXIT_FAILURE);
    for (size_t i = 0; i < cache_len; ++i) newbuf[i] = cache[i];
    free(cache);
    cache     = newbuf;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* build cache up to requested index */
    while (cache_len < (size_t)n) {
        seq_t cube = next_k * next_k * next_k;
        if (!cube_has_pal_subseq(cube)) {
            if (cache_len == cache_cap) grow_cache((cache_cap < 8) ? 8 : cache_cap * 2);
            cache[cache_len++] = next_k;
        }
        ++next_k;
    }
    return cache[(size_t)n - 1];
}
