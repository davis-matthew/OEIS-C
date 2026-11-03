/*
 * A032872  Numbers whose base-9 representation Sum_{i=0..m} d(i)*9^i has d(m)
 *          >= d(m-1) <= d(m-2) >= ...
 * https://oeis.org/A032872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Sequence A032872: a(n) = the n‑th integer whose base‑9 digits satisfy
   d_m ≥ d_{m-1} ≤ d_{m-2} ≥ d_{m-3} … with d_m the most significant digit.
   The function returns a(n) (1‑based indexing). */
static bool _is_good_base9(idx_t val)
{
    if (val == 0) return false;                 /* 0 is not in the sequence */
    int digits[64];
    int len = 0;
    idx_t tmp = val;
    while (tmp) {
        digits[len++] = (int)(tmp % 9);
        tmp /= 9;
    }
    bool ge = true;          /* first comparison should be ≥   */
    int prev = digits[len - 1];
    for (int i = len - 2; i >= 0; --i) {
        int cur = digits[i];
        if (ge) {
            if (prev < cur) return false;        /* need prev ≥ cur */
        } else {
            if (prev > cur) return false;        /* need prev ≤ cur */
        }
        ge = !ge;            /* alternate inequality */
        prev = cur;
    }
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* memoized sequence values      */
    static size_t cache_cap = 0;     /* allocated capacity             */
    static size_t cache_cnt = 0;     /* number of values cached        */
    static idx_t next_candidate = 1; /* candidate to test next we come across */

    /* generate until we have at least n terms cached */
    while (cache_cnt < n) {
        if (cache_cnt == cache_cap) { /* enlarge cache if needed */
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) abort();         /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }

        if (_is_good_base9(next_candidate)) {
            cache[cache_cnt++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
