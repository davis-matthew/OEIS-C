/*
 * A041931  Denominators of continued fraction convergents to sqrt(488).
 * https://oeis.org/A041931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 486*a(n-2) - a(n-4) for n>3 Initial values: a(0)=1,
 *            a(1)=11, a(2)=485, a(3)=5346
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Small table of the first four terms */
    static const seq_t base[4] = {1, 11, 485, 5346};

    /* Cache to make any order of calls efficient */
    static seq_t *cache  = NULL;
    static idx_t  cap    = 0;
    static idx_t  last   = (idx_t)-1;   /* largest index computed so far */

    if (n < 4)
        return base[n];

    /* Initialise the cache on the first call */
    if (!cache) {
        cap = (n >= 8) ? n + 1 : 8;
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
        if (!cache)
            return 0;                       /* allocation failure */

        cache[0] = base[0];
        cache[1] = base[1];
        cache[2] = base[2];
        cache[3] = base[3];
        last = 3;
    }

    /* Grow the cache if necessary */
    if (n >= cap) {
        idx_t new_cap = cap * 2;
        if (new_cap <= n)
            new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;                       /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute any missing values */
    if (n > last) {
        idx_t start = last == (idx_t)-1 ? 4 : last + 1;
        for (idx_t i = start; i <= n; ++i)
            cache[i] = 486 * cache[i - 2] - cache[i - 4];
        last = n;
    }

    return cache[n];
}
