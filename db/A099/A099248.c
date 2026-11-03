/*
 * A099248  Numbers such that length in binary representation and number of
 *          ones have a common divisor greater than 1.
 * https://oeis.org/A099248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9875 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by searching for numbers whose binary length and popcount have gcd > 1. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t last_scanned = 0;

    if (n == 0) return (seq_t)0;  /* outside of defined offset */

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        seq_t x = last_scanned + 1;

        /* compute bit length of x */
        unsigned bitlen = 0;
        seq_t tmp = x;
        while (tmp) {
            bitlen++;
            tmp >>= 1;
        }

        /* compute popcount of x */
        unsigned ones = 0;
        tmp = x;
        while (tmp) {
            ones += (unsigned)(tmp & (seq_t)1);
            tmp >>= 1;
        }

        if (gcd((seq_t)bitlen, (seq_t)ones) > (seq_t)1) {
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
                seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = x;
        }

        last_scanned = x;
    }

    return cache[n-1];
}
