/*
 * A163610  a(n) = ((5 + 2*sqrt(2))*(4 + sqrt(2))^n + (5 - 2*sqrt(2))*(4 -
 *          sqrt(2))^n)/2.
 * https://oeis.org/A163610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A163610: a(n) = 8*a(n-1) - 14*a(n-2), a(0) = 5, a(1) = 24 */
#include <stddef.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* static cache holding already computed terms */
    static seq_t *cache = NULL;   /* array of terms */
    static size_t capacity = 0;   /* allocated capacity */
    static size_t len = 0;        /* number of terms currently stored (>=0) */

    /* Handle the first two terms immediately */
    if (n == 0) return 5;
    if (n == 1) return 24;

    /* Initialise the cache on the very first call */
    if (!cache) {
        /* space for two initial terms */
        ensure_capacity((size_t)2, &capacity, (void **)&cache, sizeof(seq_t));
        cache[0] = 5;
        cache[1] = 24;
        len = 2;
    }

    /* If the requested term is already cached, return it */
    if ((size_t)n < len)
        return cache[n];

    /* Grow the cache to hold up to term n */
    ensure_capacity((size_t)n + 1, &capacity, (void **)&cache, sizeof(seq_t));

    /* Compute missing terms using the recurrence */
    for (idx_t i = (idx_t)len; i <= n; ++i) {
        cache[i] = 8 * cache[i - 1] - 14 * cache[i - 2];
    }

    /* Update length of stored terms */
    len = (size_t)n + 1;

    return cache[n];
}
