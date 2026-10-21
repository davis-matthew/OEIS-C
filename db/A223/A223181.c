/*
 * A223181  Rolling icosahedron footprints: number of n X 3 0..11 arrays
 *          starting with 0 where 0..11 label vertices of an icosahedron and
 *          every array movement to a horizontal, vertical or antidiagonal
 *          neighbor moves along an icosahedral edge.
 * https://oeis.org/A223181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 4*a(n-2), with initial values a(1)=25,
 *            a(2)=80, a(3)=400
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Dynamic cache: cache[i] holds a(i).  Index 0 is unused.  */
    static seq_t *cache   = NULL;   /* array of cached values      */
    static size_t cache_size = 0;   /* allocated size (elements)   */
    static size_t computed   = 0;   /* largest index that is valid */

    /* n is guaranteed to be at least 1 as per spec; guard anyway. */
    if (n < 1)
        return 0;

    /* Ensure the cache is large enough to hold up to n. */
    size_t N = (size_t)n;                  /* truncate for array size */
    if (N + 1 > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache,
                                    (N + 1) * sizeof(seq_t));
        if (!tmp) {  /* allocation failed – return 0 as a safe fallback */
            return 0;
        }
        cache = tmp;
        cache_size = N + 1;
    }

    /* Initialise base values if they have not yet been set.  */
    size_t min_base = N < 3 ? N : 3;   /* only initialise as many as we can store */
    if (computed < min_base) {
        for (size_t i = computed + 1; i <= min_base; ++i) {
            switch (i) {
                case 1: cache[1] = (seq_t)25; break;
                case 2: cache[2] = (seq_t)80; break;
                case 3: cache[3] = (seq_t)400; break;
            }
        }
        computed = min_base;
    }

    /* Compute further terms by the linear recurrence. */
    if (computed < N) {
        for (size_t i = computed + 1; i <= N; ++i) {
            cache[i] = (seq_t)6 * cache[i - 1] - (seq_t)4 * cache[i - 2];
        }
        computed = N;
    }

    return cache[N];
}
