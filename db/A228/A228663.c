/*
 * A228663  Number of 4 X n binary arrays with top left value 1 and no two
 *          ones adjacent horizontally, diagonally or antidiagonally.
 * https://oeis.org/A228663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 16*a(n-2) - 7*a(n-3) - 18*a(n-4), with a(1)=8,
 *            a(2)=12, a(3)=140, a(4)=416
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;    /* memoized values, cache[0] = a(1) */
static idx_t  cap   = 0;       /* allocated size of cache   */
static idx_t  last  = 0;       /* largest index computed */

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n < 1, but return 0 */

    /* ensure enough space for n values */
    if (cache == NULL) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) { /* allocation failure */
            return 0;
        }
    }

    if (n > cap) {
        idx_t newcap = cap;
        while (newcap < n) newcap <<= 1;          /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) { /* allocation failure */
            free(cache);
            cache = NULL;
            cap = 0;
            return 0;
        }
        cache = tmp;
        cap = newcap;
    }

    /* set initial values if not yet initialized */
    if (last < 1) cache[0] = (seq_t)8;
    if (last < 2) cache[1] = (seq_t)12;
    if (last < 3) cache[2] = (seq_t)140;
    if (last < 4) cache[3] = (seq_t)416;

    /* compute successive values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i >= 5) {
            /* recurrence: a(i) = 2*a(i-1) + 16*a(i-2) - 7*a(i-3) - 18*a(i-4) */
            seq_t left  = (seq_t)( (seq_t)2 * cache[i-2] + (seq_t)16 * cache[i-3] );
            seq_t right = (seq_t)( (seq_t)7 * cache[i-4] + (seq_t)18 * cache[i-5] );
            cache[i-1] = left - right;
        }
        /* for i == 1..4 the values are already set */
    }

    last = n;
    return cache[n-1];
}
