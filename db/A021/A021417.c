/*
 * A021417  Decimal expansion of 1/413.
 * https://oeis.org/A021417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = n-th decimal digit of 1/413
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* cached digits  */
    static size_t cap     = 0;      /* allocated capacity */
    static size_t len     = 0;      /* number of digits stored */
    static uint64_t rem    = 1;     /* remainder for next digit */

    /* Ensure the cache contains digit n */
    if ((idx_t)len <= n) {
        size_t new_len = (size_t)n + 1;

        /* Grow the cache if necessary */
        if (new_len > cap) {
            size_t new_cap = cap ? cap * 2 : 64;
            if (new_cap < new_len)
                new_cap = new_len;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                abort();            /* allocation failed */
            cache = tmp;
            cap   = new_cap;
        }

        /* Compute digits up to n */
        for (size_t i = len; i <= n; ++i) {
            rem *= 10;
            seq_t d = rem / 413;   /* next decimal digit   */
            rem %= 413;            /* new remainder        */
            cache[i] = d;
        }
        len = new_len;
    }

    return cache[(size_t)n];
}
