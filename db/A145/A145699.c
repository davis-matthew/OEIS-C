/*
 * A145699  Numbers k such that there exists x in N with (x+1)^3-x^3 = 37*k^2.
 * https://oeis.org/A145699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+2) = 590*a(n+1) - a(n) a(1) = 1, a(2) = 589
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* memoised recurrence */
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    if (n == 0) return (seq_t)0;   /* not defined for n<1 */

    size_t pos = (size_t)(n - 1); /* 0‑based index of a(n) */

    /* grow cache if necessary */
    if (pos + 1 > cap) {
        size_t new_cap = pos + 1;

        /* realloc must be cast to (seq_t*) for C++ */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure (unlikely) */

        /* initialise new elements */
        if (cap == 0) {
            tmp[0] = (seq_t)1;          /* a(1) */
            if (new_cap > 1) tmp[1] = (seq_t)589;   /* a(2) */
            for (size_t i = 2; i < new_cap; ++i)
                tmp[i] = (tmp[i-1] * (seq_t)590) - tmp[i-2];
        }
        else if (cap == 1) {
            if (new_cap > 1) tmp[1] = (seq_t)589;
            for (size_t i = 2; i < new_cap; ++i)
                tmp[i] = (tmp[i-1] * (seq_t)590) - tmp[i-2];
        }
        else {
            for (size_t i = cap; i < new_cap; ++i)
                tmp[i] = (tmp[i-1] * (seq_t)590) - tmp[i-2];
        }

        cache = tmp;
        cap   = new_cap;
    }

    return cache[pos];
}
