/*
 * A174415  Numbers such that when expressed in base 2 and then interpreted in
 *          base 10, is not a multiple of the original number.
 * https://oeis.org/A174415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers such that when expressed in base 2 and then interpreted in base 10,
   is not a multiple of the original number. This function generates the sequence
   by testing integers in order, caching results, and returning the nth term. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

static bool is_good(seq_t value)
{
    if (value == 0) return false;           /* 0 is not considered */
    unsigned bits = bitlen_seq_t(value);
    seq_t rem = 0;
    for (int i = (int)bits - 1; i >= 0; --i) {
        int bit = (int)((value >> i) & 1ULL);
        rem = (rem * (seq_t)10 + (seq_t)bit) % value;
    }
    return rem != 0;
}

static seq_t *cache   = NULL;   /* cached sequence values            */
static size_t cache_len = 0;    /* number of values stored           */
static size_t cache_cap = 0;    /* allocated array capacity          */
static idx_t cur_candidate = 0; /* last candidate checked (in idx_t) */

seq_t a(idx_t n)
{
    /* Ensure that at least n terms are cached */
    while (cache_len < n) {
        ++cur_candidate;
        seq_t val = (seq_t)cur_candidate;
        if (is_good(val)) {
            /* grow cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
#if defined(__cplusplus)
                seq_t *tmp = new seq_t[new_cap];
                for (size_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
                delete[] cache;
#else
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failed */
#endif
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
    }
    return cache[n - 1];
}
