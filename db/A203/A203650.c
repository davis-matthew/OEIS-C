/*
 * A203650  1/25 the number of (n+1) X 3 0..4 arrays with every 2 X 2 subblock
 *          having equal diagonal elements or equal antidiagonal elements.
 * https://oeis.org/A203650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 17*a(n-1) + 28*a(n-2) (n >= 3) a(1) = 81, a(2) = 1517
 *            (Sequence A203650)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;    /* cache[0] unused; cache[1] = a(1) */
static size_t cap = 0;         /* allocated capacity (size of cache) */
static idx_t  upto = 0;        /* largest n computed so far */

/* Return a(n) for n >= 1; for n == 0 returns 0 (undefined) */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;   /* undefined for n < 1 */
    }

    if (n > upto) {
        /* Ensure the cache has space for indices up to n */
        size_t needed = (size_t)n + 1;  /* +1 because index 0 is unused */

        if (needed > cap) {
            /* Grow capacity: at least needed, using doubling strategy */
            size_t new_cap = (cap > 0) ? cap : 1;
            while (new_cap < needed) {
                new_cap <<= 1;
            }
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {          /* allocation failed – return 0 */
                return (seq_t)0;
            }
            cache = tmp;
            cap   = new_cap;
        }

        /* Compute any new terms up to n */
        for (idx_t i = upto + 1; i <= n; ++i) {
            if (i == 1) {
                cache[i] = (seq_t)81;
            } else if (i == 2) {
                cache[i] = (seq_t)1517;
            } else {
                cache[i] = (seq_t)17 * cache[i-1] + (seq_t)28 * cache[i-2];
            }
        }
        upto = n;
    }

    return cache[n];
}
