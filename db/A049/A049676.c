/*
 * A049676  a(n) = (F(8*n+3) + F(8*n+1))/3, where F = A000045 (the Fibonacci
 *          sequence).
 * https://oeis.org/A049676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (F(8*n+3)+F(8*n+1))/3 Recurrence: a(0)=1, a(1)=41,
 *            a(n)=47*a(n-1)-a(n-2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* memoisation buffer */
    static size_t len  = 0;           /* number of values stored */
    static size_t cap  = 0;           /* current allocation size */

    /* make sure the buffer is large enough for index n */
    size_t needed = (size_t)n + 1;
    if (needed > cap)
        ensure_capacity(needed, &cap, (void **)&cache, sizeof(seq_t));

    /* initialise the first two terms if we have nothing yet */
    if (len == 0) {
        if (cap < 2)
            ensure_capacity(2, &cap, (void **)&cache, sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)41;
        len = 2;
    } else if (len == 1) {        /* only a(0) stored */
        if (cap < 2)
            ensure_capacity(2, &cap, (void **)&cache, sizeof(seq_t));
        cache[1] = (seq_t)41;
        len = 2;
    }

    /* compute missing values up to n */
    if ((size_t)n >= len) {
        for (size_t i = len; i <= (size_t)n; ++i) {
            cache[i] = 47 * cache[i - 1] - cache[i - 2];
        }
        len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
