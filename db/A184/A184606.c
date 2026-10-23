/*
 * A184606  Half the number of (n+1) X 2 binary arrays with no 2 X 2 subblock
 *          sum differing from a horizontal or vertical neighbor subblock sum
 *          by more than one.
 * https://oeis.org/A184606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 6*a(n-3) + 4*a(n-4); Initial values: a(1)=8,
 *            a(2)=28, a(3)=98, a(4)=350
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cached values, 0‑based: cache[i] = a(i+1) */
static idx_t  cache_n = 0;    /* number of cached terms */

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* safety, though n>=1 by definition */

    /* enlarge cache if needed */
    if (n > cache_n) {
        seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!new_cache) return 0;   /* out of memory, return 0 as guard */
        cache = new_cache;

        /* fill new entries */
        for (idx_t i = cache_n; i < n; ++i) {
            idx_t idx = i + 1;  /* 1‑based index */

            if (idx == 1)      cache[i] = (seq_t)8;
            else if (idx == 2) cache[i] = (seq_t)28;
            else if (idx == 3) cache[i] = (seq_t)98;
            else if (idx == 4) cache[i] = (seq_t)350;
            else {
                /* recurrence: a(n) = 3*a(n-1) + 6*a(n-3) + 4*a(n-4) */
                seq_t val = 3 * cache[i - 1];           /* a(n-1) */
                val += 6 * cache[i - 3];               /* a(n-3) */
                val += 4 * cache[i - 4];               /* a(n-4) */
                cache[i] = val;
            }
        }
        cache_n = n;
    }

    return cache[n - 1];
}
