/*
 * A206337  Number of (n+1) X 3 0..3 arrays with every 2 X 2 subblock having
 *          nonzero determinant and commuting with every horizontal or
 *          vertical neighbor.
 * https://oeis.org/A206337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 33, u64 n <= 73, u128 n <= 154,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 3*a(n-2) - 3*a(n-3) for n>11
   Initial values: a(1)=96, a(2)=181, a(3)=232, a(4)=534, a(5)=686,
   a(6)=1589, a(7)=2042, a(8)=4751, a(9)=6110,
   a(10)=14239, a(11)=18318 */
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* Handle the trivial case safely */
    if (n == 0) return (seq_t)0;

    /* Ensure we have room for indices 0..n */
    size_t need = (size_t)(n + 1);          /* +1 for 0-based array */
    size_t old_cap = cache_cap;             /* capacity before growth */

    if (cache_cap < need) {
        seq_t *tmp = (seq_t*)realloc(cache, need * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);       /* allocation failed */
        cache      = tmp;
        cache_cap  = need;
    }

    /* Initial values for n = 1..11 (index 0 unused, set to 0) */
    static const seq_t init[12] = {0,
        96, 181, 232, 534, 686, 1589, 2042, 4751, 6110, 14239, 18318
    };

    /* Compute any terms that are not yet stored */
    for (size_t i = old_cap; i <= (size_t)n; ++i) {
        if (i <= 11) {
            cache[i] = init[(size_t)i];
        } else {
            /* a(i) = a(i-1) + 3*a(i-2) - 3*a(i-3) */
            cache[i] = cache[i-1] + cache[i-2] * (seq_t)3 - cache[i-3] * (seq_t)3;
        }
    }

    return cache[(size_t)n];
}
