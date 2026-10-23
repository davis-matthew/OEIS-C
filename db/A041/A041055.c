/*
 * A041055  Denominators of continued fraction convergents to sqrt(33).
 * https://oeis.org/A041055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 46*a(n-4)-a(n-8) for n ≥ 8 Initial values: a(0)=1,
 *            a(1)=1, a(2)=3, a(3)=4, a(4)=43, a(5)=47, a(6)=137, a(7)=184
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated size */
    static size_t len   = 0;   /* number of valid entries in cache */

    /* base values for indices 0..7 */
    static const seq_t base[8] = {1, 1, 3, 4, 43, 47, 137, 184};

    /* convert idx_t to size_t safely (n is ≤ 10000 in tests) */
    size_t N = (size_t)n;

    /* ensure cache has room up to index N */
    while (len <= N) {
        /* grow the cache if needed */
        if (len >= cap) {
            size_t newcap = (cap == 0 ? 8 : cap << 1);
            if (newcap <= len) newcap = len + 1;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) { /* allocation failure – abort */
                abort();
            }
            cache = tmp;
            cap = newcap;
        }

        /* compute next term */
        if (len <= 7) {
            cache[len] = base[len];
        } else {
            cache[len] = (seq_t)46 * cache[len - 4] - cache[len - 8];
        }
        len++;
    }

    return cache[N];
}
