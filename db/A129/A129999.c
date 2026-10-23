/*
 * A129999  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+337)^2 = y^2.
 * https://oeis.org/A129999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 6*a(n-3) - a(n-6) + 674 for n > 6 Initial values: a(1)=0,
 *            a(2)=27, a(3)=888, a(4)=1011, a(5)=1148, a(6)=6027
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Protect against accidental calls with n == 0 */
    if (n == 0)
        return (seq_t)0;

    /* Cached sequence values: cache[i] holds a(i) */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;        /* allocated capacity (>= last index +1) */
    static idx_t   last   = 0;        /* highest index computed so far */

    /* Ensure enough room for index n */
    if (cap < (size_t)n + 1) {
        size_t newcap = cap ? cap * 2 : 16;
        while (newcap <= (size_t)n)
            newcap <<= 1;              /* grow until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failure */
        cache = tmp;
        /* initialise new elements to 0 */
        for (size_t i = cap; i < newcap; ++i)
            cache[i] = (seq_t)0;
        cap = newcap;
    }

    /* Initialise first six terms if not already done */
    if (last < 6 && n > last) {
        static const seq_t init[6] = { 0, 27, 888, 1011, 1148, 6027 };
        size_t start = (size_t)last + 1;
        size_t end   = (size_t)(n < 6 ? n : 6);
        for (size_t i = start; i <= end; ++i)
            cache[i] = init[i - 1];
        last = (idx_t)end;
    }

    /* Compute missing terms using the recurrence */
    if (n > last) {
        for (size_t i = (size_t)last + 1; i <= (size_t)n; ++i) {
            seq_t a_nm3 = cache[i - 3];
            seq_t a_nm6 = cache[i - 6];
            seq_t val   = 6 * a_nm3 - a_nm6 + (seq_t)674;
            cache[i] = val;
        }
        last = n;
    }

    return cache[n];
}
