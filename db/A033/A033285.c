/*
 * A033285  Numbers whose maximal base-10 run length is 4.
 * https://oeis.org/A033285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 104, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 48 terms through n = 971 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose maximal base-10 run length is 4.
   The algorithm generates integers in increasing order,
   keeps a cache of the already produced terms, and
   returns the nth element.  The property test scans
   the decimal digits of each candidate. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true iff every run of equal digits in v has length
   at most 4, and there exists at least one run of length exactly 4. */
static bool has_max_run_four(seq_t v) {
    if (v == 0) return false;               /* should never be called for 0 */
    unsigned prev_digit = 10;               /* invalid initial digit */
    unsigned curr_len = 0;
    unsigned max_len = 0;
    bool found_four = false;

    seq_t t = v;
    while (t > 0) {
        unsigned d = (unsigned)(t % 10);
        if (d == prev_digit) {
            curr_len++;
        } else {
            if (curr_len > max_len) {
                max_len = curr_len;
                if (max_len > 4) return false;
                found_four = (max_len == 4);
            }
            curr_len = 1;
            prev_digit = d;
        }
        t /= 10;
    }
    /* final run */
    if (curr_len > max_len) {
        max_len = curr_len;
        if (max_len > 4) return false;
        found_four = (max_len == 4);
    }
    return (found_four && max_len == 4);
}

/* External function required by the build system */
seq_t a(idx_t n) {
    /* Cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_n   = 0;      /* number of entries in cache */
    static seq_t next_candidate = 0; /* last candidate tested */

    /* Initialise cache once */
    if (cache == NULL) {
        cache_cap = 16;
        cache    = mem_new(seq_t, cache_cap);
    }

    /* Generate terms until we have at least n of them */
    while (cache_n < n) {
        next_candidate++;                  /* next integer to test  */
        if (has_max_run_four(next_candidate)) {
            /* Grow cache if needed */
            if (cache_n == cache_cap) {
                cache_cap *= 2;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_n++] = next_candidate;
        }
    }
    return cache[n-1];
}
