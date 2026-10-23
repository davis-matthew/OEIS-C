/*
 * A141525  a(n) = a(n-2) + a(n-3) if n == 0 (mod 3), a(n-1) + a(n-4) if n ==
 *          0 (mod 4), otherwise a(n-2) with a(0) = 0 and a(1) = a(2) = a(3) =
 *          1.
 * https://oeis.org/A141525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=a(2)=a(3)=1; a(n) = a(n-2) + a(n-3) if n ≡ 0 (mod
 *            3) a(n) = a(n-1) + a(n-4) if n ≡ 0 (mod 4) a(n) = a(n-1)
 *            otherwise (exact integer arithmetic, memoised)
 * Range:     u8 n <= 26, u16 n <= 50, u32 n <= 92, u64 n <= 182,
 *            u128 n <= 359, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static idx_t   last   = 0;

    /* initialise cache with known start values */
    if (cache == NULL) {
        cap = 4;                         /* indices 0..3 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;             /* OOM: return zero */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        last = 3;
    }

    /* enlarge cache if necessary */
    if (n >= cap) {
        size_t new_cap = cap;
        while (n >= new_cap)
            new_cap *= 2;

        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;             /* OOM: return zero */

        for (size_t i = 0; i < cap; ++i)
            new_cache[i] = cache[i];
        for (size_t i = cap; i < new_cap; ++i)
            new_cache[i] = (seq_t)0;

        free(cache);
        cache = new_cache;
        cap = new_cap;
    }

    /* value already cached */
    if (n <= last)
        return cache[n];

    /* compute missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val;
        if (i % 3 == 0) {                /* divisible by 3 */
            val = cache[i - 2] + cache[i - 3];
        } else if (i % 4 == 0) {          /* divisible by 4 */
            val = cache[i - 1] + cache[i - 4];
        } else {                          /* otherwise */
            val = cache[i - 1];
        }
        cache[i] = val;
    }
    last = n;
    return cache[n];
}
