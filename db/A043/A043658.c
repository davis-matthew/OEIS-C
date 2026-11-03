/*
 * A043658  Numbers whose base-13 representation has exactly 3 runs.
 * https://oeis.org/A043658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043658: Numbers whose base-13 representation has exactly 3 runs.
   The function returns the n-th term (1‑based) of this sequence.  An integer is
   counted if, when written in base 13, its digits consist of exactly three
   maximal blocks of equal digits (e.g. 1 0 1, 12 0 3, 1 11 1, etc.).
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static inline bool has_three_runs(seq_t x) {
    int runs = 0;
    int prev = -1;
    while (x > 0) {
        int d = (int)(x % 13);
        if (d != prev) {
            ++runs;
            if (runs > 3) return false;
            prev = d;
        }
        x /= 13;
    }
    return runs == 3;
}

/* static cache to serve repeated calls in any order */
static seq_t  *cache = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static idx_t  last_checked = 169;   /* start just before the first term (170) */

/* append a new term, resizing storage if necessary */
static void append_value(seq_t v) {
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_ptr = mem_resize(seq_t, cache, new_cap);
        if (!new_ptr) exit(1);                     /* out of memory */
        cache = new_ptr;
        cache_cap = new_cap;
    }
    cache[cache_len++] = v;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                      /* undefined, defensive */

    while (cache_len < (size_t)n) {
        ++last_checked;                       /* test next integer */
        if (has_three_runs((seq_t)last_checked)) {
            append_value((seq_t)last_checked);
        }
    }
    return cache[n - 1];
}
