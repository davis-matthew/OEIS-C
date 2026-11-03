/*
 * A012493  Take every 5th term of Padovan sequence A000931, beginning with
 *          the fifth term.
 * https://oeis.org/A012493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = 5*a(n+2) - 4*a(n+1) + a(n), a(0)=0, a(1)=2, a(2)=9
 *            Source: OEIS A012493 – every 5th Padovan term starting with the
 *            fifth term
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 960 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Handle the three initial values directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)9;

    /* Static cache of computed terms. */
    static seq_t *cache = NULL;
    static idx_t cached = 0;          /* number of terms currently stored; cache[0..cached-1] valid */

    /* Ensure the cache is large enough to contain the requested term. */
    if (cached <= 2) {
        /* initialise cache with the first three terms */
        cached = 3;
        cache = mem_new(seq_t, cached);
        if (!cache) abort();          /* out‑of‑memory */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)9;
    }

    if (n + 1 < cached) return cache[n];

    /* Grow the cache to n+1 elements. */
    idx_t new_size = n + 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_size);
    if (!tmp) abort();
    cache = tmp;

    /* Compute any missing terms using the recurrence. */
    for (idx_t i = cached; i <= n; ++i) {
        cache[i] = 5 * cache[i - 1] - 4 * cache[i - 2] + cache[i - 3];
    }
    cached = new_size;

    return cache[n];
}
