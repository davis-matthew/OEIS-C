/*
 * A218752  a(n) = (50^n - 1)/49.
 * https://oeis.org/A218752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (50^n - 1)/49 Recurrence: a(0) = 0, a(n) = 50*a(n-1) + 1
 *            Computes the terms of OEIS sequence A218752
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 600 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */
#include "../utils/mem.h"

static seq_t *cache = NULL;      /* pointer to computed terms */
static idx_t  cached_size = 0;   /* number of stored terms (max index + 1) */

seq_t a(idx_t n)
{
    /* Initialise cache with a(0) if necessary */
    if (cached_size == 0) {
        cache = mem_new(seq_t, 1);
        if (!cache) return 0;          /* allocation failure → zero */
        cache[0] = 0;                  /* a(0) = 0 */
        cached_size = 1;
    }

    /* Extend cache if requested index is beyond current bounds */
    if (n >= cached_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, (size_t)new_size);
        if (!tmp) return 0;            /* allocation failure → zero */
        cache = tmp;

        /* Fill new entries using the recurrence */
        for (idx_t i = cached_size; i <= n; ++i) {
            cache[i] = (seq_t)50 * cache[i - 1] + 1;
        }
        cached_size = new_size;
    }

    return cache[n];
}
