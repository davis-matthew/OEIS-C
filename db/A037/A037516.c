/*
 * A037516  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,0,1.
 * https://oeis.org/A037516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A037516: Base-7 digits are, in order, the first n terms of the periodic
    sequence 2,0,1,2,0,1,…
    a(1)=2;
    a(n)=a(n-1)*7 + d_n  for n>1, where d_n is 2,0,1 repeating (d_1=2).
    This recurrence computes the required terms iteratively. */

#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of values, index 1..cap-1 */
    static size_t cap   = 0;      /* allocated capacity */
    static idx_t stored = 0;      /* highest index computed so far */

    if (n == 0)                /* offset < 1 – value irrelevant */
        return 0;

    /* make sure cache can hold entries up to index n */
    ensure_cache(n, &cache, &cap);

    /* initial entry a(1) = 2 */
    if (stored == 0) {
        cache[1] = 2;
        stored   = 1;
    }

    /* compute missing terms up to n */
    if (n > stored) {
        static const int pat[3] = {2, 0, 1};
        for (idx_t i = stored + 1; i <= n; ++i) {
            int d = pat[(i - 1) % 3];
            cache[i] = cache[i - 1] * 7 + (seq_t)d;
        }
        stored = n;
    }

    return cache[n];
}
