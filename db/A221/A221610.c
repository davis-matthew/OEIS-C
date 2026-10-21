/*
 * A221610  Number of 2 X n arrays of occupancy after each element moves to
 *          some horizontal or antidiagonal neighbor, without move-in move-out
 *          left turns.
 * https://oeis.org/A221610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 9*a(n-2) - 5*a(n-3) + 10*a(n-4) - 3*a(n-5),
 *            with a(1)=0, a(2)=2, a(3)=10, a(4)=53, a(5)=269, a(6)=1378,
 *            a(7)=7057
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values, index 0 corresponds to a(1) */
    static seq_t *cache = NULL;
    static idx_t stored = 0;           /* number of values currently stored */

    if (n == 0)                       /* undefined, but return 0 for safety */
        return (seq_t)0;

    /* enlarge cache if necessary */
    if (stored < n) {
        seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!new_cache)              /* allocation failure: return 0 as a fallback */
            return (seq_t)0;
        cache = new_cache;

        /* compute missing terms */
        for (idx_t i = stored; i < n; ++i) {    /* i is 0‑based: 0 => a(1) */
            if (i == 0)
                cache[i] = (seq_t)0;
            else if (i == 1)
                cache[i] = (seq_t)2;
            else if (i == 2)
                cache[i] = (seq_t)10;
            else if (i == 3)
                cache[i] = (seq_t)53;
            else if (i == 4)
                cache[i] = (seq_t)269;
            else if (i == 5)
                cache[i] = (seq_t)1378;
            else if (i == 6)
                cache[i] = (seq_t)7057;
            else
                /* recurrence: a(n) = 7*a(n-1) - 9*a(n-2) - 5*a(n-3) + 10*a(n-4) - 3*a(n-5) */
                cache[i] = (seq_t)7 * cache[i-1]
                         - (seq_t)9 * cache[i-2]
                         - (seq_t)5 * cache[i-3]
                         + (seq_t)10 * cache[i-4]
                         - (seq_t)3 * cache[i-5];
        }
        stored = n;
    }

    return cache[n-1];
}
