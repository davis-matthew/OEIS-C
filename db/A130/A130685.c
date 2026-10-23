/*
 * A130685  a(0)=0; a(n) = n-th integer from among those positive integers
 *          which are coprime to (a(n-1) + n).
 * https://oeis.org/A130685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(n) = the n‑th positive integer coprime to (a(n-1)+n)
 *            for n≥1 (i.e. the n‑th element of the sequence of positive
 *            integers that are coprime to a(n‑1)+n)
 * Range:     u8 n <= 58, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static idx_t  max_ndx = 0;

    /* Grow the cache if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap << 1 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;

#ifdef __cplusplus
        /* C++: use new[] and copy values */
        seq_t *tmp = new seq_t[new_cap];
        if (cache) {
            for (size_t i = 0; i < cap; ++i)
                tmp[i] = cache[i];
            delete[] cache;
        }
        cache = tmp;
        cap   = new_cap;
#else
        /* C: use realloc (trivially copyable) */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failure */
        cache = tmp;
        cap   = new_cap;
#endif
    }

    /* Return cached value if already computed */
    if (n <= max_ndx)
        return cache[n];

    /* Initialise a(0) on first run */
    if (max_ndx == 0) {
        cache[0] = 0;
        max_ndx = 0;
    }

    /* Compute successive terms up to n */
    for (idx_t i = max_ndx + 1; i <= n; ++i) {
        /* Compute M = a(i-1) + i */
        seq_t M = cache[i-1] + (seq_t)i;

        /* Find the i‑th positive integer coprime to M */
        seq_t candidate = 1;
        idx_t  count    = 0;

        while (count < i) {
            if (gcd(candidate, M) == 1) {
                ++count;
            }
            ++candidate;
        }
        /* The loop exits when count == i, so candidate-1 is the result */
        cache[i] = candidate - 1;
    }

    max_ndx = n;
    return cache[n];
}
