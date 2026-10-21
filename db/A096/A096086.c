/*
 * A096086  a(n) = 153*a(n-1) - 3984*a(n-2) - 37*a(n-3).
 * https://oeis.org/A096086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 153*a(n-1) - 3984*a(n-2) - 37*a(n-3) with a(1) = 3, a(2)
 *            = 825, a(3) = 114236
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[1] holds a(1), ... */
    static idx_t cap   = 0;       /* allocated capacity (including extra slots) */
    static idx_t last  = 0;       /* largest n already computed */

    if (n == 0) return 0;        /* offset starts at 1 – undefined otherwise */

    /* first-time initialization */
    if (cache == NULL)
    {
        cap = (n > 4 ? n : 4) + 10;          /* room for at least three base terms */
        cache = (seq_t *)malloc((cap + 1) * sizeof(seq_t)); /* index 0 unused */
        if (cache == NULL) return 0;           /* allocation failure – unlikely */

        cache[1] = (seq_t)3;
        cache[2] = (seq_t)825;
        cache[3] = (seq_t)114236;
        last = 3;
    }

    /* enlarge the cache if necessary */
    if (n > cap)
    {
        idx_t new_cap = n + 10;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (tmp == NULL) return 0;            /* allocation failure – unlikely */
        cache = tmp;
        cap = new_cap;
    }

    if (n <= last)                /* already computed */
        return cache[n];

    /* compute terms from last+1 up to n */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        seq_t t1 = (seq_t)153 * cache[i - 1];
        seq_t t2 = (seq_t)3984 * cache[i - 2];
        seq_t t3 = (seq_t)37   * cache[i - 3];
        cache[i] = t1 - t2 - t3;
    }

    last = n;
    return cache[n];
}
