/*
 * A223182  Rolling icosahedron footprints: number of n X 4 0..11 arrays
 *          starting with 0 where 0..11 label vertices of an icosahedron and
 *          every array movement to a horizontal, vertical or antidiagonal
 *          neighbor moves along an icosahedral edge.
 * https://oeis.org/A223182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 25*a(n-2) + 36*a(n-3) - 24*a(n-4) + 4*a(n-5)
 *            Initial values: a(1)=125, a(2)=320, a(3)=2080, a(4)=14560,
 *            a(5)=103520, a(6)=738720
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Lazy memoization; works for any idx_t, seq_t types. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static const seq_t init[6] = {
        (seq_t)125,
        (seq_t)320,
        (seq_t)2080,
        (seq_t)14560,
        (seq_t)103520,
        (seq_t)738720
    };

    if (n == 0)
        return (seq_t)0;   /* not used, but safe */

    if ((size_t)n > cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp;
        if (cache == NULL)
            tmp = seq_alloc(new_cap);
        else
            tmp = seq_realloc(cache, cap, new_cap);

        if (!tmp)
            return (seq_t)0; /* allocation failure */

        cache = tmp;
        /* fill new entries */
        for (size_t idx = cap; idx < new_cap; ++idx) {
            if (idx < 6) {
                cache[idx] = init[idx];
            } else {
                /* recurrence: a(n) = 10*a(n-1) - 25*a(n-2) + 36*a(n-3) - 24*a(n-4) + 4*a(n-5) */
                seq_t a1 = cache[idx-1];
                seq_t a2 = cache[idx-2];
                seq_t a3 = cache[idx-3];
                seq_t a4 = cache[idx-4];
                seq_t a5 = cache[idx-5];
                cache[idx] = (10*a1) - (25*a2) + (36*a3) - (24*a4) + (4*a5);
            }
        }
        cap = new_cap;
    }
    return cache[n-1];
}
