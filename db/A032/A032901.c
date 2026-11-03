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
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-03)
 * Helpers:   mem
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
#include "../utils/mem.h"

static bool valid_base4(seq_t v)
{
    seq_t tmp = v;
    int prev = (int)(tmp % 4);
    tmp /= 4;
    bool leq = true;            /* next comparison is ≤ */

    while (tmp > 0) {
        int cur = (int)(tmp % 4);
        if (leq) {
            if (prev > cur) return false;
        } else {
            if (prev < cur) return false;
        }
        leq = !leq;
        prev = cur;
        tmp /= 4;
    }
    return true;
}

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
        cache = mem_new(seq_t, cap);
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
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) abort();
            cache = tmp;
            cap = new_cap;
        }
        cache[len++] = last;
    }

    return cache[n - 1];
}
