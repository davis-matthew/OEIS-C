/*
 * A177814  Numbers k such that k^3 divides 14^(k^2) + 1.
 * https://oeis.org/A177814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 12, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   mod_pow, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A177814: numbers k such that k^3 | 14^(k^2) + 1 */

/*  The function searches k successively, keeps all found terms in a
    static cache and returns the (n‑1)th cached value (offset 1).               */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mod_pow.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache of all found k values */
    static seq_t *cache      = NULL;   /* pointer to array of k */
    static size_t cache_size = 0;      /* number of cached terms */
    static size_t cache_cap  = 0;      /* allocated capacity      */
    static idx_t  cur_k       = 1;     /* next k to test          */

    if (n == 0) return 0;              /* offset is 1; harmless */
    
    /* extend cache until it contains at least n terms */
    while (cache_size < (size_t)n) {
        /* small optimization: skip k that are even or a multiple of 7,
           because 14^(k^2)+1 cannot then be divisible by k^3          */
        if ((cur_k & 1ULL) == 0 || cur_k % 7ULL == 0) {
            ++cur_k;
            continue;
        }

        seq_t  k   = (seq_t)cur_k;
        seq_t  mod = k * k * k;                     /* k^3          */
        uint64_t exp = (uint64_t)cur_k * (uint64_t)cur_k;  /* k^2          */

        /* 14^(k^2) (mod k^3) */
        seq_t val = mod_pow((seq_t)14, exp, mod);

        /* check the divisibility condition */
        if (val == mod - 1) {
            /* ensure enough space in cache */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) abort();     /* allocation failed */
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = k;
        }
        ++cur_k;
    }

    return cache[n - 1];
}
