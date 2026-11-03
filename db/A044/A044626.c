/*
 * A044626  Numbers n such that string 7,2 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS def: Numbers n such that the octal string "72"
   occurs in the octal representation of n but not of n+1. */

/* Implements the sequence by searching for the next qualifying
   integer and memoizing results so that arbitrary queries are fast. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: returns true if the octal representation of val contains
   the consecutive digits 7 followed by 2. */
static bool has_72(seq_t val) {
    if (val == 0) return false;
    seq_t tmp = val;
    /* Enough for any 64‑bit number, and far more than needed for
       the first few thousand terms. */
    uint8_t digits[256];
    size_t d = 0;
    while (tmp > 0) {
        digits[d++] = (uint8_t)(tmp % 8);
        tmp /= 8;
    }
    for (size_t i = d; i > 1; --i) {
        if (digits[i - 1] == 7 && digits[i - 2] == 2)
            return true;
    }
    return false;
}

/* Dynamic cache for the sequence values. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of values stored */
static size_t cache_cap = 0;   /* allocated capacity */

/* Allocate or grow the cache. */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t newcap = cache_cap ? cache_cap * 2 : 4;
    while (newcap < needed)
        newcap *= 2;
    seq_t *newbuf = mem_new(seq_t, newcap);
    if (!newbuf) abort();          /* out of memory */
    if (cache) {
        mem_copy(newbuf, cache, cache_len * sizeof(seq_t));
        mem_free(cache);
    }
    cache = newbuf;
    cache_cap = newcap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) abort();          /* offset is 1; n>=1 assumed */
    /* Grow cache until we have at least n terms. */
    while (cache_len < (size_t)n) {
        seq_t candidate;
        if (cache_len == 0)
            candidate = 1;
        else
            candidate = cache[cache_len - 1] + 1;
        /* Search for next qualifying number. */
        for (;;) {
            if (has_72(candidate) && !has_72(candidate + 1)) {
                ensure_cache(cache_len + 1);
                cache[cache_len++] = candidate;
                break;
            }
            candidate += 1;
        }
    }
    return cache[n - 1];
}
