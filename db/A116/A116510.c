/*
 * A116510  a(0)=1. a(m +2^n) = a(m) - a(n), 0 <= m <= 2^n -1.
 * https://oeis.org/A116510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 120, i16 n <= 120, i32 n <= 120, i64 n <= 120,
 *            i128 n <= 120, big any n
 * Helpers:   flr_log2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0) = 1;
   For all n >= 0 and 0 <= m < 2^n:  a(m + 2^n) = a(m) - a(n) */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/flr_log2.h"

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* enlarge cache if necessary */
    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();      /* allocation failed */
        cache = tmp;
        if (cache_size == 0) {   /* initialise a(0) */
            cache[0] = (seq_t)1;
            cache_size = 1;
        }
        /* fill in new entries */
        for (idx_t i = cache_size; i <= n; ++i) {
            idx_t p = flr_log2(i);          /* highest power of two <= i */
            idx_t base = ((idx_t)1) << p;   /* 2^p */
            idx_t m = i - base;             /* 0 <= m < base */
            cache[i] = cache[m] - cache[p];
        }
        cache_size = new_size;
    }
    return cache[n];
}
