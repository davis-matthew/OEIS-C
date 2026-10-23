/*
 * A032901  Numbers whose base-4 representation Sum_{i=0..m} d(i)*4^i has d(0)
 *          <= d(1) >= d(2) <= ...
 * https://oeis.org/A032901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   valid_base4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A032901: numbers whose base‑4 digits satisfy
    d(0) <= d(1) >= d(2) <= d(3) >= …
    (alternating ≤ and ≥ starting with ≤).
    The function generates the sequence on demand and caches
    previously computed terms.  */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/valid_base4.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static seq_t last = 0;       /* last examined number */

    /* initialise cache once */
    if (!cache) {
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* produce terms until we have at least n terms cached */
    while (len < n) {
        ++last;
        if (!valid_base4(last))
            continue;

        /* grow cache if needed */
        if (len == cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cap = new_cap;
        }
        cache[len++] = last;
    }

    return cache[n - 1];
}
