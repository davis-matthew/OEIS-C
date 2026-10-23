/*
 * A041053  Denominators of continued fraction convergents to sqrt(32).
 * https://oeis.org/A041053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 34*a(n-4) - a(n-8) Initial values: a(0)=1, a(1)=1,
 *            a(2)=2, a(3)=3, a(4)=32, a(5)=35, a(6)=67, a(7)=102
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoisation table */
    static size_t cap   = 0;      /* current capacity of cache */
    static size_t len   = 0;      /* number of terms stored */

    size_t need = (size_t)n + 1;  /* we need indices 0 .. n inclusive */

    /* enlarge the cache if necessary */
    if (need > cap) {
        size_t new_cap = cap ? cap << 1 : 8;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms */
    if (len < need) {
        static const seq_t init[8] = {1, 1, 2, 3, 32, 35, 67, 102};
        for (size_t i = len; i < need; ++i) {
            if (i < 8)
                cache[i] = init[i];
            else
                cache[i] = 34 * cache[i-4] - cache[i-8];
        }
        len = need;
    }

    return cache[n];
}
