/*
 * A217022  Number of city-block distance 1, pressure limit 2 movements in an
 *          n X 2 array with each element moving exactly one horizontally or
 *          vertically and no element acquiring more than two neighbors.
 * https://oeis.org/A217022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 3*a(n-2) - 25*a(n-3) + 9*a(n-4) + 3*a(n-5) -
 *            a(n-6) (n > 6), with initial values a(1)=1, a(2)=16, a(3)=121,
 *            a(4)=1024, a(5)=8464, a(6)=70225
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;         /* memoised values: cache[i] == a(i+1) */
static size_t len   = 0;            /* number of computed entries */
static size_t cap   = 0;            /* allocated capacity */

static const seq_t init[6] = {
    (seq_t)1,      /* a(1) */
    (seq_t)16,     /* a(2) */
    (seq_t)121,    /* a(3) */
    (seq_t)1024,   /* a(4) */
    (seq_t)8464,   /* a(5) */
    (seq_t)70225   /* a(6) */
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for n<1 */

    /* Grow the cache if needed */
    if (cap < (size_t)n) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute values up to a(n) if not already done */
    for (size_t i = len; i < (size_t)n; ++i) {
        if (i < 6) {
            cache[i] = init[i];
        } else {
            seq_t v =
                (  9 * cache[i - 1]
                 - 3 * cache[i - 2]
                 - 25 * cache[i - 3]
                 + 9 * cache[i - 4]
                 + 3 * cache[i - 5]
                 - cache[i - 6] );
            cache[i] = v;
        }
    }
    len = (size_t)n;

    return cache[(size_t)n - 1];
}
