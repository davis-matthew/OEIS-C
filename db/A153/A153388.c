/*
 * A153388  Second bisection of A153382.
 * https://oeis.org/A153388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + a(n-3); Initial values: a(0)=8,
 *            a(1)=18, a(2)=48
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t   cap   = 0;     /* size of allocated array */
    static idx_t   computed = 0;   /* number of entries already computed */

    /* If we already have the answer, we can return immediately. */
    if (n < computed) return cache[n];

    /* Make sure the array can hold index n. */
    ensure_cache(n, &cache, &cap);

    /* Initialize base values if not yet present. */
    if (computed == 0) {
        /* set values up to min(2, n) */
        cache[0] = (seq_t)8;
        computed = 1;
        if (n >= 1) {
            cache[1] = (seq_t)18;
            computed = 2;
        }
        if (n >= 2) {
            cache[2] = (seq_t)48;
            computed = 3;
        }
    } else {
        /* If some of the first three terms were not computed before,
           compute the missing ones while staying within requested n. */
        if (n >= 1 && computed <= 1) {
            cache[1] = (seq_t)18;
            computed = 2;
        }
        if (n >= 2 && computed <= 2) {
            cache[2] = (seq_t)48;
            computed = 3;
        }
    }

    /* Compute remaining terms using the linear recurrence. */
    for (idx_t i = computed; i <= n; ++i) {
        cache[i] = 4 * cache[i - 1] - 4 * cache[i - 2] + cache[i - 3];
    }
    computed = n + 1;

    return cache[n];
}
