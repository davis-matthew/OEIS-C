/*
 * A076708  Numbers k such that triangular numbers T(k) and T(k+1) sum to
 *          another triangular number (that is also a perfect square).
 * https://oeis.org/A076708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula/Recurrence:
   a(n) satisfies a(n+2) = 6*a(n+1) - a(n) + 4,  with a(1)=0, a(2)=5
   (offset 1).  The function memoises values so any order of calls is efficient. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* safety, though n≥1 is guaranteed */

    /* static cache of computed values, indexed by the sequence offset 1 */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;              /* number of allocated slots (index 0 unused) */

    /* initialise cache on first call */
    if (cache_len == 0) {
        cache_len = 3;                       /* store indices 0,1,2 */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 0;                        /* unused */
        cache[1] = 0;                        /* a(1) */
        cache[2] = 5;                        /* a(2) */
    }

    /* grow cache to include requested index */
    if ((idx_t)n >= cache_len) {
        idx_t new_len = n + 1;               /* allocate up to index n inclusive */
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val = (seq_t)6 * cache[i-1] - cache[i-2] + (seq_t)4;
            cache[i] = val;
        }
        cache_len = new_len;
    }

    return cache[n];
}
