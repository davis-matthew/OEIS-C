/*
 * A178769  a(n) = (5*10^n + 13)/9.
 * https://oeis.org/A178769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*10^n + 13)/9
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 904 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=2, a(1)=7, a(n)=10*a(n-1)-13 for n>1 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  last_index = 0;
    static bool   initialized = false;

    /* initialize cache on first call */
    if (!initialized) {
        /* allocate space for indices 0 and 1 */
        cap = 2;
        cache = alloc_seq_array(cap);

        /* set first two terms */
        cache[0] = (seq_t)2;          /* a(0) */
        cache[1] = (seq_t)7;          /* a(1) */

        last_index   = 1;
        initialized  = true;
    }

    /* grow cache if needed */
    if ((size_t)n + 1 > cap) {
        grow_seq(&cache, &cap, (size_t)n + 1);
    }

    /* compute missing terms up to n */
    if (last_index < n) {
        for (idx_t i = last_index + 1; i <= n; ++i) {
            cache[i] = cache[i - 1] * (seq_t)10 - (seq_t)13;
        }
        last_index = n;
    }

    return cache[n];
}
