/*
 * A126866  a(n) = 13*a(n-1) - a(n-2).
 * https://oeis.org/A126866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 13*a(n-1) - a(n-2), a(0) = 1, a(1) = 14
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 889 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Compute the OEIS sequence A126866.  This implementation
   uses a simple dynamic cache to support repeated calls in any
   order.  All arithmetic is done in seq_t, which may be an
   arbitrarily‑precise integer class or any unsigned integer
   type.  The cache is resized lazily and the recurrence
   is evaluated iteratively. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t  len   = 0;

    /* initialise cache with first two terms */
    if (len == 0) {
        cache = mem_new(seq_t, 2);
        cache[0] = 1;
        cache[1] = 14;
        len = 2;
    }

    /* already computed? */
    if (n < len)
        return cache[n];

    /* extend cache up to n (inclusive) */
    idx_t old_len = len;
    len = n + 1;
    cache = mem_resize(seq_t, cache, len);

    /* compute missing terms using the recurrence */
    for (idx_t i = old_len; i <= n; ++i)
        cache[i] = 13 * cache[i - 1] - cache[i - 2];

    return cache[n];
}
