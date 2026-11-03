/*
 * A062826  Square nialpdromes not ending in 0.
 * https://oeis.org/A062826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A062826: squares of non‑increasing decimal numbers
 * not ending in 0.  The term is computed by a linear search over
 * integer roots, memoising the results so repeated calls are fast.
 * Formula (by definition): 
 *   a(n) = the n‑th number k such that
 *          k = m*m,  m has non‑increasing decimal digits, and
 *          k % 10 != 0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Test whether the decimal representation of x has non‑increasing digits
 * (left‑to‑right). */
static bool non_increasing_digits(seq_t x)
{
    /* 2^128 has 39 decimal digits; a uint8_t array of length 80 is ample */
    uint8_t digs[80];
    size_t dlen = 0;

    if (x == 0)
        return true;

    seq_t t = x;
    while (t > 0) {
        digs[dlen++] = (uint8_t)(t % 10);
        t /= 10;
    }

    /* digits[0] is least significant, digits[dlen‑1] most significant.
       Check that each more significant digit is >= the next one. */
    for (size_t i = dlen - 1; i > 0; --i) {
        if (digs[i] < digs[i - 1])
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* static memoisation cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t next_root = 1;    /* root candidate to test next */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    }

    /* generate terms until we have at least n */
    while (cache_len < n) {
        seq_t sq = (seq_t)next_root * (seq_t)next_root;

        if ((sq % 10) != 0 && non_increasing_digits(sq)) {
            /* grow cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 1;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = sq;
        }
        ++next_root;
    }
    return cache[n - 1];
}
