/*
 * A048693  Generalized Pellian with 2nd term equal to 6.
 * https://oeis.org/A048693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2); a(0)=1, a(1)=6
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 30 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static idx_t  cap     = 0;
    static idx_t  last    = 0;

    if (cache == NULL) {
        cap   = 2;
        cache = mem_new(seq_t, cap);
        if (!cache) return (seq_t)0;              /* allocation failure */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        last    = 1;
    }

    if (n <= last)
        return cache[n];

    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        size_t newcap = cap;
        while (newcap < needed)
            newcap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, newcap);
        if (!tmp) return (seq_t)0;                /* allocation failure */
        cache = tmp;
        cap   = newcap;
    }

    for (idx_t i = last + 1; i <= n; ++i)
        cache[i] = 2 * cache[i - 1] + cache[i - 2];

    last = n;
    return cache[n];
}
