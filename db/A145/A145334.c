/*
 * A145334  Numbers x such that (x+43)^3-x^3 is a square.
 * https://oeis.org/A145334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+2) = 33710*a(n+1) - a(n) + 724722
   Initial terms: a(1) = 1118,  a(2) = 38413663 */

#include <stdlib.h>
#include <stdint.h>   /* for uintptr_t if needed */

/* Return the n-th term of OEIS A145334.  n>=1.  For n<1 the
   behaviour is undefined; we return 0 in that case.  The function
   memoises computed values in a static array so that calls may
   come in any order. */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;   /* array of computed terms */
    static idx_t  cached_n = 0;     /* number of terms currently cached */
    static size_t cache_cap = 0;    /* allocated capacity of cache */

    if (n == 0) return 0;          /* undefined for n=0, return 0 */

    /* extend cache up to n */
    while (cached_n < n) {
        idx_t cur = cached_n + 1;   /* current term index (1‑based) */
        seq_t val;

        if (cur == 1) {
            val = 1118;
        } else if (cur == 2) {
            val = 38413663;
        } else {
            /* recurrence: a(cur) = 33710*a(cur-1) - a(cur-2) + 724722
               cache is 0‑based, so a(cur-1) -> cache[cur-2], a(cur-2) ->
               cache[cur-3] */
            val = 33710ULL * cache[cur - 2] - cache[cur - 3] + 724722ULL;
        }

        /* grow cache if needed */
        if (cached_n == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 4;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                free(cache);
                exit(EXIT_FAILURE);
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cached_n++] = val;
    }

    return cache[n - 1];
}
