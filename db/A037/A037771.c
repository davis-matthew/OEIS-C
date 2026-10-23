/*
 * A037771  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 3,0,2,1.
 * https://oeis.org/A037771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10*a(n-1) + a(n-4) - 10*a(n-5) initial values: a(1)=3,
 *            a(2)=30, a(3)=302, a(4)=3021, a(5)=30213
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache: holds a[0] .. a[cap-1] corresponding to a(1)..a(cap) */
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    if (n == 0) return (seq_t)0;   /* safety; n ≥ 1 in correct use */

    if ((size_t)n > cap) {
        size_t new_cap = (size_t)n;
        seq_t *new_cache = seq_realloc(cache, cap, new_cap);
        cache = new_cache;

        for (size_t i = cap; i < new_cap; ++i) {
            if (i == 0)           /* a(1) */
                cache[i] = (seq_t)3;
            else if (i == 1)      /* a(2) */
                cache[i] = (seq_t)30;
            else if (i == 2)      /* a(3) */
                cache[i] = (seq_t)302;
            else if (i == 3)      /* a(4) */
                cache[i] = (seq_t)3021;
            else if (i == 4)      /* a(5) */
                cache[i] = (seq_t)30213;
            else {
                /* recurrence: a(n) = 10*a(n-1)+a(n-4)-10*a(n-5) */
                seq_t t1 = cache[i-1];
                seq_t t4 = cache[i-4];
                seq_t t5 = cache[i-5];
                cache[i] = t1 * (seq_t)10 + t4 - t5 * (seq_t)10;
            }
        }
        cap = new_cap;
    }

    return cache[n-1];   /* since cache[0] is a(1) */
}
