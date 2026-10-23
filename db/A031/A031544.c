/*
 * A031544  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 46.
 * https://oeis.org/A031544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the n-th positive integer k such that the continued
 * fraction expansion of sqrt(k) has even period, the periodic part ends
 * with 2·floor(sqrt(k)), and after removing that last term the
 * central term of the remaining period equals 46.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"   /* provides: seq_t floor_sqrt(seq_t) */

static bool satisfies(seq_t k)
{
    seq_t a0 = floor_sqrt(k);
    if (a0 * a0 == k)                /* skip perfect squares */
        return false;

    /* First pass: compute period length */
    seq_t m = (seq_t)0, d = (seq_t)1, a = a0;
    idx_t len = (idx_t)0;
    while (true) {
        m = d * a - m;
        d = (k - m * m) / d;
        a = (a0 + m) / d;           /* next periodic term */
        len++;
        if (a == 2 * a0)             /* period completed */
            break;
    }

    if ((len & 1) != 0)              /* period must be even */
        return false;

    idx_t mid_index = (len - (idx_t)2) / (idx_t)2;   /* 0‑based, after last term removed */

    /* Second pass: locate the middle term */
    m = (seq_t)0;
    d = (seq_t)1;
    a = a0;
    for (idx_t j = (idx_t)0; j <= mid_index; ++j) {
        m = d * a - m;
        d = (k - m * m) / d;
        a = (a0 + m) / d;
        if (j == mid_index)
            return a == (seq_t)46;
    }
    return false;   /* should never reach here */
}

static seq_t *cache = NULL;
static idx_t cache_cap = (idx_t)0;
static idx_t cache_len = (idx_t)0;
static seq_t next_k = (seq_t)3;       /* start scanning from 3 */

static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * (idx_t)2 : (idx_t)8;
    while (new_cap < needed)
        new_cap *= (idx_t)2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* allocation failed – stop execution */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Grow cache until it contains at least n entries */
    while (cache_len < n) {
        seq_t kval = next_k;
        if (satisfies(kval)) {
            ensure_capacity(cache_len + (idx_t)1);
            cache[cache_len++] = kval;
        }
        next_k++;
    }
    return cache[n - (idx_t)1];
}
