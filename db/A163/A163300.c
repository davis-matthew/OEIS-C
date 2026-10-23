/*
 * A163300  Even numbers without 2.
 * https://oeis.org/A163300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*A087156(n). Equivalently, a(1)=0, a(2)=4, a(3)=6, and
 *            for n>=4 a(n) = 2*a(n-1) - a(n-2)
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;      /* cache[1..] stores a(k) */
static size_t cache_cap = 0;     /* allocated capacity (cache_cap>=1) */
static idx_t computed = 0;      /* highest index computed so far */

/* Ensure cache has entries up to n, computing as needed. */
static void ensure_cache(idx_t n)
{
    if (n <= computed)
        return;

    /* Allocate or grow storage */
    size_t new_cap = (size_t)(n + 1);
    if (new_cap > cache_cap) {
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (cache && cache_cap > 0)
            memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)0;
        } else if (i == 2) {
            cache[2] = (seq_t)4;
        } else if (i == 3) {
            cache[3] = (seq_t)6;
        } else {
            /* recurrence: a(n) = 2*a(n-1) - a(n-2) */
            cache[i] = (seq_t)(cache[i - 1] * (seq_t)2 - cache[i - 2]);
        }
    }
    computed = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* behaviour below offset is irrelevant */
    ensure_cache(n);
    return cache[n];
}
