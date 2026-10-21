/*
 * A056957  In repeated iterations of function m->m/2 if m even, m->3m-1 if m
 *          odd, a(n) is minimum value achieved if starting from n.
 * https://oeis.org/A056957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   ensure_capacity, seq_min
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A056957:  a(n) is the smallest value reached when repeatedly
 * applying the rule: m → m/2 if m is even, m → 3m–1 if m is odd,
 * starting from n.  The function memoises results for speed. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/ensure_capacity.h"
#include "../utils/seq_min.h"

#define VISITED_CAP 1024

static seq_t *cache = NULL;      /* cache[n‑1] = a(n) for n ≤ cache_cap */
static size_t cache_cap = 0;     /* current cache capacity */

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* not expected, but safe */

    /* Ensure cache can hold the result for n */
    if ((size_t)n > cache_cap) {
        size_t old_cap = cache_cap;
        size_t new_cap = (size_t)n;
        ensure_capacity(new_cap, &cache_cap, (void **)&cache, sizeof(seq_t));
        /* initialise newly allocated slots to zero (sentinel) */
        for (size_t i = old_cap; i < cache_cap; ++i) cache[i] = 0;
    }

    if (cache[n - 1] != 0) return cache[n - 1];

    /* Cycle detection buffer for the current trajectory */
    static seq_t visited[VISITED_CAP];
    size_t visited_len = 0;

    seq_t cur = (seq_t)n;
    seq_t cur_min = cur;

    while (1) {
        /* If we have already computed the minimal value for cur, use it */
        if (cur <= (seq_t)cache_cap && cache[(idx_t)(cur - 1)] != 0) {
            cur_min = seq_min(cur_min, cache[(idx_t)(cur - 1)]);
            break;
        }

        /* Check if cur has appeared earlier in this run (cycle) */
        for (size_t i = 0; i < visited_len; ++i) {
            if (visited[i] == cur) {
                /* Cycle detected; cur_min already holds the minimum of the cycle */
                goto done;
            }
        }

        /* Record current value and update minimum */
        if (visited_len < VISITED_CAP) visited[visited_len++] = cur;
        /* In practice, cycle length for n ≤ 10000 is < VISITED_CAP */

        if (cur < cur_min) cur_min = cur;

        /* Next transformation */
        if ((cur & 1) == 0) {
            cur = cur >> 1;           /* even → m/2 */
        } else {
            cur = cur * 3 - 1;        /* odd  → 3m–1 */
        }
    }

done:
    cache[n - 1] = cur_min;  /* memoise result */
    return cur_min;
}
