/*
 * A208224  a(n)=(a(n-1)^2*a(n-3)^3+a(n-2))/a(n-4) with a(0)=a(1)=a(2)=a(3)=1.
 * https://oeis.org/A208224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = (a(n-1)^2 * a(n-3)^3 + a(n-2)) / a(n-4),
   a(0)=a(1)=a(2)=a(3)=1 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;   /* storage for computed terms   */
static size_t len   = 0;      /* first unavailable index      */
static size_t cap   = 0;      /* allocated capacity          */

/* Ensure that cache contains terms 0 … n (inclusive) */
static void ensure_cache(idx_t n)
{
    /* Grow the storage if necessary   */
    if (cap == 0) {
        cap   = 8;
        cache = seq_alloc(cap);
        if (!cache) abort();
    }
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = seq_realloc(cache, cap, new_cap);
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute every term that is still missing    */
    for (size_t i = len; i <= (size_t)n; ++i) {
        if (i < 4) {
            cache[i] = (seq_t)1;
        } else {
            seq_t a1 = cache[i-1];
            seq_t a2 = cache[i-2];
            seq_t a3 = cache[i-3];
            seq_t a4 = cache[i-4];
            seq_t term = ((a1 * a1) * (a3 * a3 * a3) + a2) / a4;
            cache[i] = term;
        }
    }
    len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
