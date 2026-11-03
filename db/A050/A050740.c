/*
 * A050740  Numbers of form 11^k (values of k see A050731) containing no pair
 *          of consecutive equal digits (probably finite).
 * https://oeis.org/A050740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 8 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Series:  a(n) = 11^k_n  where k_n is the n‑th exponent such that
 *  11^k_n contains no pair of consecutive equal digits.
 *  The sequence is generated on demand and cached.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether a seq_t value has any pair of consecutive equal decimal digits. */
static inline bool no_consecutive_equal_digits(seq_t val) {
    if (val == (seq_t)0) return true;          /* 0 contains only one digit */
    int prev = -1;                             /* previous digit, -1 means none yet */
    seq_t tmp = val;
    while (tmp > (seq_t)0) {
        int d = (int)(tmp % (seq_t)10);        /* current least‑significant digit */
        if (prev == d) return false;           /* found equal neighbours */
        prev = d;
        tmp /= (seq_t)10;                      /* shift right one decimal digit */
    }
    return true;                               /* all adjacent digits differ */
}

seq_t a(idx_t n)
{
    /* Cache of found terms */
    static seq_t *cache      = NULL;
    static size_t cache_sz   = 0;
    static size_t cache_cap  = 0;
    /* Current power 11^k */
    static seq_t power        = (seq_t)1;  /* 11^0 */
    static int   initialized  = 0;

    if (!initialized) {
        cache      = NULL;
        cache_sz   = 0;
        cache_cap  = 0;
        power      = (seq_t)1;
        initialized = 1;
    }

    /* Grow cache until it holds indices 0 … n */
    while (cache_sz <= n) {
        if (no_consecutive_equal_digits(power)) {
            if (cache_sz == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) return (seq_t)0;   /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = power;
        }
        /* Next power of 11 */
        power = power * (seq_t)11;
    }
    return cache[n];
}
