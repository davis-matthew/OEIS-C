/*
 * A221083  Number of n X 4 arrays of occupancy after each element moves to
 *          some horizontal or antidiagonal neighbor.
 * https://oeis.org/A221083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=4, a(2)=169, a(3)=6889, a(n)=45*a(n-1)-180*a(n-2)+64*a(n-3)
 *            for n≥4
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[1..cache_len] holds already computed terms */
static idx_t cache_len = 0;      /* largest n for which cache[n] is valid */

seq_t a(idx_t n)
{
    /* n >= 1 is the valid domain of the sequence.  The function may
       be called with n==0; in that case we simply return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Grow the cache up to n if necessary */
    if (cache_len < n) {
        /* The cast is required for C++ compliance; realloc() returns void* */
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!new_cache) {
            /* allocation failed – abort to avoid a silent failure */
            abort();
        }
        cache = new_cache;

        /* Compute new terms */
        idx_t start = cache_len + 1;          /* first index that needs filling */
        if (start < 1) start = 1;              /* safety for the case cache_len==0 */

        for (idx_t j = start; j <= n; ++j) {
            if (j == 1)
                cache[j] = (seq_t)4;
            else if (j == 2)
                cache[j] = (seq_t)169;
            else if (j == 3)
                cache[j] = (seq_t)6889;
            else {
                /* Apply the linear recurrence */
                cache[j] = (seq_t)45 * cache[j-1]
                         - (seq_t)180 * cache[j-2]
                         + (seq_t)64 * cache[j-3];
            }
        }
        cache_len = n;
    }

    return cache[n];
}
