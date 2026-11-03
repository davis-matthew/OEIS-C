/*
 * A031524  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 26.
 * https://oeis.org/A031524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A031524
   k such that the continued fraction for sqrt(k) has an even period and,
   after deleting the last term of the period, the central term equals 26. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Return the term at position pos (1‑based) in the periodic part. */
static seq_t period_term(seq_t k, seq_t a0, unsigned pos) {
    seq_t m = (seq_t)0, d = (seq_t)1, a = a0;
    for (unsigned i = 1; i <= pos; ++i) {
        seq_t mnext = d * a - m;
        seq_t dnext = (k - mnext * mnext) / d;
        seq_t anext = (a0 + mnext) / dnext;
        m = mnext; d = dnext; a = anext;
    }
    return a;
}

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_checked = 1;   /* next k to examine */

/* Return the period length of the continued fraction of sqrt(k). */
static unsigned period_length(seq_t k, seq_t a0) {
    seq_t m = (seq_t)0, d = (seq_t)1, a = a0;
    unsigned len = 0;
    do {
        seq_t mnext = d * a - m;
        seq_t dnext = (k - mnext * mnext) / d;
        seq_t anext = (a0 + mnext) / dnext;
        m = mnext; d = dnext; a = anext;
        ++len;
    } while (a != ((seq_t)2) * a0);
    return len;
}

/* Expand the cache to at least new_cap elements. */
static void expand_cache(size_t new_cap) {
    if (new_cap <= cache_cap) return;
    seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Generate terms until we have at least n elements in cache. */
static void generate_until(idx_t n) {
    while (cache_len < n) {
        seq_t k = (seq_t)last_checked++;
        seq_t a0 = floor_sqrt(k);
        if (a0 * a0 == k) continue;                 /* k is a perfect square */
        unsigned per = period_length(k, a0);
        if ((per & 1) != 0) continue;               /* period must be even */
        seq_t mid = period_term(k, a0, per / 2);
        if (mid != (seq_t)26) continue;             /* central term must be 26 */
        /* Store k in cache. */
        if (cache_len == cache_cap) expand_cache(cache_cap ? cache_cap * 2 : 64);
        cache[cache_len++] = k;
    }
}

/* Public function required by the build system. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* behaviour below offset irrelevant */
    if (n > cache_len) generate_until(n);
    return cache[n - 1];
}
