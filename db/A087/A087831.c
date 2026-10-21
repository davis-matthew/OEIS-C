/*
 * A087831  a(n) = a(abs(n-a(n-1))) + a(a(abs(n-a(n-4)))) if n>4, a(n) = 1
 *          otherwise.
 * https://oeis.org/A087831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(abs(n-a(n-1))) + a(a(abs(n-a(n-4)))) for n>4 a(n) = 1
 *            for n<=4
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   abs_diff_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h> /* needed only for abort() in out-of-memory */
#include "../utils/abs_diff_idx.h"

static seq_t *cache = NULL;        /* cached values of a(n) */
static idx_t cache_len = 0;       /* number of elements stored */
static idx_t cache_cap = 0;       /* allocated capacity */

/* grow the cache to hold at least 'need' elements */
static void ensure_cap(idx_t need)
{
    if (need <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp) abort();          /* OOM – terminate */
    cache = newp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < cache_len)
        return cache[n];

    ensure_cap(n + 1);

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i <= 4) {
            cache[i] = 1;                  /* base values */
            continue;
        }
        /* first term: a(abs(n-a(n-1))) */
        idx_t d1 = abs_diff_idx(i, cache[i - 1]);
        seq_t t1 = cache[d1];

        /* second term: a(a(abs(n-a(n-4)))) */
        idx_t d2 = abs_diff_idx(i, cache[i - 4]);
        idx_t a_of_d2 = (idx_t)cache[d2];   /* value becomes an index */
        seq_t t2 = cache[a_of_d2];

        cache[i] = t1 + t2;
    }
    cache_len = n + 1;
    return cache[n];
}
