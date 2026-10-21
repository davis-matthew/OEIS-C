/*
 * A234061  Number of (n+2)X(1+2) 0..3 arrays with no increasing sequence of
 *          length 3 horizontally or diagonally downwards.
 * https://oeis.org/A234061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 60*a(n-1) -139*a(n-2) -1552*a(n-3) +3544*a(n-4)
 *            +640*a(n-5) -4720*a(n-6) +2048*a(n-7) (for n > 7, a(1) = 205808,
 *            a(2) = 11758836, a(3) = 671548560, a(4) = 38351813588, a(5) =
 *            2190245258368, a(6) = 125083358211012, a(7) = 7143421916769712)
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 8, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;                 /* offset is 1 */

    static seq_t *cache   = NULL;         /* indexed 1.. */
    static size_t cap     = 0;            /* cache capacity */
    static idx_t  last_max = 0;           /* largest n computed so far */

    /* ensure array large enough */
    size_t need = (size_t)n + 1;          /* position n + dummy 0 */
    if (need > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) return (seq_t)0;       /* allocation failure */
        if (!cache) {                    /* first allocation */
            for (size_t i = 0; i < need; ++i)
                tmp[i] = (seq_t)0;
        } else {                         /* new region to zero */
            for (size_t i = cap; i < need; ++i)
                tmp[i] = (seq_t)0;
        }
        cache = tmp;
        cap   = need;
    }

    /* initialise first seven terms */
    static const seq_t init[8] = {
        0ULL,                      /* dummy for index 0 */
        205808ULL,                 /* a(1) */
        11758836ULL,               /* a(2) */
        671548560ULL,              /* a(3) */
        38351813588ULL,            /* a(4) */
        2190245258368ULL,          /* a(5) */
        125083358211012ULL,        /* a(6) */
        7143421916769712ULL        /* a(7) */
    };
    for (idx_t i = (idx_t)1; i <= (idx_t)7 && i <= n; ++i)
        if (cache[(size_t)i] == (seq_t)0)
            cache[(size_t)i] = init[(size_t)i];

    /* trivial case for n <= 7 */
    if (n <= (idx_t)7)
        return cache[(size_t)n];

    /* extend computation using the recurrence */
    if (last_max < (idx_t)7) last_max = (idx_t)7;
    if (last_max < n) {
        for (idx_t i = last_max + (idx_t)1; i <= n; ++i) {
            seq_t v = 0;
            v += (seq_t)60    * cache[(size_t)(i - 1)];
            v -= (seq_t)139   * cache[(size_t)(i - 2)];
            v -= (seq_t)1552  * cache[(size_t)(i - 3)];
            v += (seq_t)3544  * cache[(size_t)(i - 4)];
            v += (seq_t)640   * cache[(size_t)(i - 5)];
            v -= (seq_t)4720  * cache[(size_t)(i - 6)];
            v += (seq_t)2048  * cache[(size_t)(i - 7)];
            cache[(size_t)i] = v;
        }
        last_max = n;
    }
    return cache[(size_t)n];
}
