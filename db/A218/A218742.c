/*
 * A218742  a(n) = (39^n - 1)/38.
 * https://oeis.org/A218742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (39^n - 1)/38
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 573 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed via the linear recurrence: a(0)=0, a(1)=1, a(n)=40*a(n-1)-39*a(n-2) */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

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
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t up_to   = 0;   /* highest index already computed */

    /* Ensure capacity for n+1 entries */
    if (n + 1 > cache_cap) {
        grow_seq(&cache, &cache_cap, n + 1);
    }

    /* initialise base values (overwriting any stale data) */
    cache[0] = 0;
    if (n >= 1) {
        cache[1] = 1;
    }

    /* compute forward if we haven't yet reached n */
    if (up_to < n) {
        idx_t start = (up_to >= 1) ? up_to + 1 : 2;   /* recurrence starts at 2 */
        for (idx_t i = start; i <= n; ++i) {
            cache[i] = cache[i-1] * 40 - cache[i-2] * 39;
        }
        up_to = n;
    }

    return cache[n];
}
