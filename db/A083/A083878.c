/*
 * A083878  a(0)=1, a(1)=3, for n >= 2, a(n) = 6*a(n-1) - 7*a(n-2).
 * https://oeis.org/A083878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=3, a(n)=6*a(n-1)-7*a(n-2) for n>=2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static idx_t cached_sz = 0;

    if (n >= cached_sz) {
        /* grow the cache to at least n+1 entries */
        idx_t new_sz = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failed – return zero */
        cache = tmp;

        /* initialise if this is the first allocation */
        if (cached_sz == 0) {
            cache[0] = 1;
            cached_sz = 1;
        }
        if (cached_sz == 1 && n >= 1) {
            cache[1] = 3;
            cached_sz = 2;
        }

        /* compute missing terms using the recurrence */
        for (idx_t i = cached_sz; i <= n; ++i) {
            cache[i] = 6 * cache[i-1] - 7 * cache[i-2];
        }
        cached_sz = new_sz;
    }

    return cache[n];
}
