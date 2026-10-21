/*
 * A052910  Expansion of 1 + 2/(1-2*x-x^3).
 * https://oeis.org/A052910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-3), a(0)=1, a(1)=2, a(2)=4, a(3)=8
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 56, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* array of precomputed values */
    static idx_t len  = 0;          /* number of values actually stored  */
    static idx_t cap  = 0;          /* allocated capacity of the array */

    if (cache == NULL) {
        /* initialise the first four terms */
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* fallback if allocation fails */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)8;

        len = 4;
    }

    if (n >= len) {
        /* make sure we have enough space */
        if (n + 1 > cap) {
            idx_t newcap = (cap > 0) ? cap : 4;
            while (n + 1 > newcap) newcap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* fallback on failure */
            cache = tmp;
            cap   = newcap;
        }

        /* compute missing terms up to n using the recurrence */
        for (idx_t i = len; i <= n; ++i) {
            cache[i] = cache[i - 1] * ((seq_t)2) + cache[i - 3];
        }
        len = n + 1;
    }

    return cache[n];
}
