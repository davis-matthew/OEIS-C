/*
 * A085889  7-smooth numbers (A002473) using digits in increasing order. 9 is
 *          followed by a zero.
 * https://oeis.org/A085889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 15 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A085889: 7‑smooth numbers whose decimal digits form an
   increasing consecutive sequence (9 may be followed immediately
   by 0).  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true if the decimal digits of x are consecutive
   in ascending order (wrapping 9→0 allowed).  */
static bool digits_consecutive(seq_t x)
{
    /* Store digits least‑to‑most; 200 is plenty for any seq_t. */
    uint32_t digits[200];
    int len = 0;

    if (x == 0) {
        /* 0 is technically not a 7‑smooth number, but the
           check is never called for it.  */
        return true;
    }
    while (x > 0) {
        digits[len++] = (uint32_t)(x % 10);
        x /= 10;
    }
    if (len <= 1) return true;   /* single digit is always consecutive */

    uint32_t prev = digits[len - 1];          /* most significant digit */
    for (int i = len - 2; i >= 0; --i) {
        uint32_t cur = digits[i];
        if (prev == 9) {
            if (cur != 0) return false;      /* 9 must be followed by 0 */
        } else {
            if (cur != prev + 1) return false; /* otherwise it must +1 */
        }
        prev = cur;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* static state for incremental generation */
    static seq_t *hum      = NULL;   /* all generated 7‑smooth numbers */
    static size_t hum_cap = 0, hum_sz = 0;
    static size_t i2 = 0, i3 = 0, i5 = 0, i7 = 0;

    static seq_t *vals      = NULL;   /* qualifying numbers */
    static size_t vals_cap = 0, vals_sz = 0;

    /* first call: initialise arrays */
    if (hum == NULL) {
        hum_cap = 1024;
        hum = mem_new(seq_t, hum_cap);
        hum[0] = 1;
        hum_sz = 1;
        i2 = i3 = i5 = i7 = 0;

        vals_cap = 256;
        vals = mem_new(seq_t, vals_cap);
        vals[0] = 1;           /* 1 is the first qualifying number */
        vals_sz = 1;
    }

    /* generate until we have at least n qualifying numbers */
    while (vals_sz < (size_t)n) {
        seq_t cand2 = hum[i2] * 2;
        seq_t cand3 = hum[i3] * 3;
        seq_t cand5 = hum[i5] * 5;
        seq_t cand7 = hum[i7] * 7;

        seq_t next = cand2;
        if (cand3 < next) next = cand3;
        if (cand5 < next) next = cand5;
        if (cand7 < next) next = cand7;

        /* append new humble number */
        if (hum_sz == hum_cap) {
            hum_cap <<= 1;
            hum = mem_resize(seq_t, hum, hum_cap);
        }
        hum[hum_sz++] = next;

        /* advance pointers that produced this value */
        if (next == cand2) ++i2;
        if (next == cand3) ++i3;
        if (next == cand5) ++i5;
        if (next == cand7) ++i7;

        /* check the digit property */
        if (digits_consecutive(next)) {
            if (vals_sz == vals_cap) {
                vals_cap <<= 1;
                vals = mem_resize(seq_t, vals, vals_cap);
            }
            vals[vals_sz++] = next;
        }
    }

    return vals[n - 1];
}
