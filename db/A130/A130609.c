/*
 * A130609  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+223)^2 = y^2.
 * https://oeis.org/A130609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) + 446 for n > 6; a(1)=0, a(2)=32,
 *            a(3)=533, a(4)=669, a(5)=833, a(6)=3672
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 29, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle undefined offsets gracefully. */
    if (n == 0)
        return (seq_t)0;

    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated size */
    static size_t used  = 0;   /* number of stored terms */

    /* ensure capacity for n terms */
    if (cap < (size_t)n) {
        size_t newcap = n;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)    /* allocation failure – fall back */
            return (seq_t)0;
        cache = tmp;
        cap   = newcap;
    }

    /* initialise the first six terms on first use */
    if (used < 6) {
        /* make sure we have room for the six base terms */
        if (cap < 6) {
            size_t newcap = 6;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp)
                return (seq_t)0;
            cache = tmp;
            cap = newcap;
        }
        static const seq_t init[6] = {0, 32, 533, 669, 833, 3672};
        for (size_t i = used; i < 6; ++i)
            cache[i] = init[i];
        used = 6;
    }

    /* compute terms up to the requested index */
    if (n > used) {
        for (size_t i = used; i < (size_t)n; ++i) {
            /* recurrence: a(i+1) = 6*a(i-2) - a(i-5) + 446 */
            cache[i] = 6 * cache[i - 3] - cache[i - 6] + (seq_t)446;
        }
        used = (size_t)n;
    }

    return cache[n - 1];
}
