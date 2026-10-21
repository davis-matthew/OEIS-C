/*
 * A218748  a(n) = (45^n - 1)/44.
 * https://oeis.org/A218748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (45^n - 1)/44 Recurrence: a(0)=0, a(n)=45*a(n-1)+1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;
static size_t cap   = 0;   /* allocated capacity, size_t matches ensure_capacity */
static idx_t len   = 0;    /* number of stored terms */

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (len == 0) {
        ensure_capacity(1, &cap, (void **)&cache, sizeof(seq_t));
        cache[0] = (seq_t)0;
        len = 1;
    }

    /* return cached value if available */
    if (n < len)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        seq_t val = cache[i-1] * (seq_t)45 + (seq_t)1;
        ensure_capacity(len + 1, &cap, (void **)&cache, sizeof(seq_t));
        cache[len] = val;
        ++len;
    }

    return cache[n];
}
