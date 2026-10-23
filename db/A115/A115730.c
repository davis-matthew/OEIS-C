/*
 * A115730  a(n) = a(n-3) + A001654(n-1) with a(0)=0, a(1)=0 and a(2)=1.
 * https://oeis.org/A115730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) + A001654(n-1) Implemented via equivalent linear
 *            recurrence: a(n) = 2*a(n-1) + 2*a(n-2) - 2*a(n-4) - 2*a(n-5) +
 *            a(n-6), with base values a(0)=0, a(1)=0, a(2)=1, a(3)=2, a(4)=6,
 *            a(5)=16, a(6)=42
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 48, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial terms */
    static const seq_t base[7] = {0, 0, 1, 2, 6, 16, 42};

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t capacity = 0;   /* allocated size */
    static size_t len      = 0;   /* number of terms stored */

    /* fast return for small n */
    if (n < 7)
        return base[(size_t)n];

    /* initialise cache on first call */
    if (!cache) {
        capacity = 7;
        cache = (seq_t *)malloc(sizeof(seq_t) * capacity);
        for (size_t i = 0; i < 7; ++i)
            cache[i] = base[i];
        len = 7;
    }

    /* enlarge cache if required */
    if ((size_t)n >= capacity) {
        size_t new_cap = capacity ? capacity << 1 : 8;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache = tmp;
        capacity = new_cap;
    }

    /* compute new terms up to n */
    if ((size_t)n >= len) {
        for (size_t i = len; i <= (size_t)n; ++i) {
            seq_t term = cache[i - 6];

            /* term += 2*a[i-1] + 2*a[i-2] - 2*a[i-4] - 2*a[i-5] */
            term += cache[i - 1];
            term += cache[i - 1];
            term += cache[i - 2];
            term += cache[i - 2];
            term -= cache[i - 4];
            term -= cache[i - 4];
            term -= cache[i - 5];
            term -= cache[i - 5];

            cache[i] = term;
        }
        len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
