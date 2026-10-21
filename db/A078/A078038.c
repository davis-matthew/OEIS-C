/*
 * A078038  Expansion of (1-x)/(1+x-2*x^2-x^3).
 * https://oeis.org/A078038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n+3) = -a(n+2) + 2*a(n+1) + a(n) * Initial values: a(0)=1,
 *            a(1)=-2, a(2)=4 * The function memoises results so it works in *
 *            arbitrary call order and runs in O(n) time per new element
 * Range:     i8 n <= 7, i16 n <= 17, i32 n <= 36, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* memoised values */
static idx_t cached_size = 0;   /* number of elements stored */

/* External linkage required by the build system */
seq_t a(idx_t n)
{
    /* Base cases solved without any allocation. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-2;
    if (n == 2) return (seq_t)4;

    /* Initialise the cache on first use */
    if (cache == NULL)
    {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-2;
        cache[2] = (seq_t)4;
        cached_size = 3;
    }

    /* Extend the cache if the requested index is beyond the current size */
    if (n >= cached_size)
    {
        /* Resize the array to accommodate up to n */
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) /* allocation failed – abort with null pointer */
            return 0;   /* behaviour undefined, but prevents double free */
        cache = tmp;

        /* Compute new elements using the recurrence */
        for (idx_t i = cached_size; i <= n; ++i)
        {
            seq_t val = -cache[i - 1]
                      + 2 * cache[i - 2]
                      + cache[i - 3];
            cache[i] = val;
        }
        cached_size = n + 1;
    }

    return cache[n];
}
