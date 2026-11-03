/*
 * A044784  Numbers n such that string 7,1 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044784: Numbers n such that the decimal string "71"
   occurs in n but not in n+1. This implementation searches
   sequentially and memoises results; no closed‑form formula is used. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Helper: returns true iff x contains the digit pattern 71 (most‑significant first). */
static bool contains_71(seq_t x)
{
    /* 128-bit numbers have at most 39 decimal digits; 50 is plenty. */
    static const size_t MAX_DIGITS = 50;
    unsigned char digits[MAX_DIGITS];
    size_t len = 0;
    seq_t t = x;

    if (t == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (t > (seq_t)0) {
            digits[len++] = (unsigned char)(t % (seq_t)10);
            t /= (seq_t)10;
        }
    }

    for (size_t i = 1; i < len; ++i)
        if (digits[i] == 7 && digits[i-1] == 1)
            return true;

    return false;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static seq_t cur_candidate = 1;

    if (n == 0)  /* offset is 1; return 0 for n==0 as a safe default. */
        return (seq_t)0;

    if (!cache) {
        cache_cap = 128;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) return (seq_t)0;
    }

    while (cache_size < n) {
        if (contains_71(cur_candidate) && !contains_71(cur_candidate + (seq_t)1)) {
            if (cache_size == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                if (!tmp) { mem_free(cache); cache = NULL; return (seq_t)0; }
                cache = tmp;
            }
            cache[cache_size++] = cur_candidate;
        }
        cur_candidate++;
    }
    return cache[n - 1];
}
