/*
 * A173277  A(x) satisfies A000290(x)/x^2 = A(x)/A(x^2); A000290 = integer
 *          squares.
 * https://oeis.org/A173277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=4, a(3)=13 and for n ≥ 4 a(n) = a(floor((n+1)/2)) +
 *            3*a(n‑1) - 3*a(n‑2) + a(n‑3)
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* MIT License.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache – grows as needed.  */
    static seq_t *cache   = NULL;
    static size_t   cap   = 0;   /* # of allocated elements            */
    static idx_t    len   = 0;   /* highest computed index + 1         */

    if (n == 0)            /* index 0 is not part of the OEIS offset */
        return 0;

    /* Grow the buffer if necessary.  The explicit cast is required
       to compile as C++ when seq_t is a class type.           */
    if ((idx_t)cap <= n) {
        size_t new_cap = cap ? cap * 2 : 16;
        if (new_cap <= (size_t)n)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Out of memory – abort quietly (undefined in spec). */
            return 0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* If the cache hasn't yet been initialized, write the first
       four known values.  For n ≥ 4 the recurrence will fill the rest. */
    if (len == 0) {
        cache[0] = 0;   /* used only for the recurrence              */
        cache[1] = 1;   /* a(1)                                      */
        cache[2] = 4;   /* a(2)                                      */
        cache[3] = 13;  /* a(3)                                      */
        len = 4;
    }

    /* Compute values up to the requested index n. */
    for (idx_t i = len; i <= n; ++i) {
        if (i == 0)          cache[0] = 0;
        else if (i == 1)     cache[1] = 1;
        else if (i == 2)     cache[2] = 4;
        else if (i == 3)     cache[3] = 13;
        else {
            /* recurrence: a(i) = a(floor((i+1)/2)) + 3*a(i-1)
               - 3*a(i-2) + a(i-3)                           */
            seq_t rhs = cache[(i + 1) / 2];
            cache[i] = rhs + 3 * cache[i - 1] - 3 * cache[i - 2] + cache[i - 3];
        }
    }
    len = n + 1;

    return cache[n];
}
