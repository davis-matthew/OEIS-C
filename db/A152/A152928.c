/*
 * A152928  Number of sets (in the Hausdorff metric geometry) at each location
 *          between two sets defining a polygonal configuration consisting of
 *          two m-gonal polygonal components chained with string components of
 *          length 1 as m varies.
 * https://oeis.org/A152928
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 8*a(n-2) + a(n-3) (n > 4) Initial values:
 *            a(2)=113, a(3)=765, a(4)=5234, a(5)=35865
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>


seq_t a(idx_t n)
{
    static const idx_t OFFSET = 2;                /* offset of the sequence */

    if (n < OFFSET)          /* behaviour below the offset is irrelevant   */
        return 0;

    size_t needed = (size_t)(n - OFFSET + 1);     /* number of elements needed   */
    static seq_t *cache = NULL;                  /* memoisation array          */
    static size_t  cache_size = 0;                /* current size of cache      */
    size_t old_size = cache_size;                 /* size before possible grow  */

    /* Grow cache if necessary (shrink not performed) */
    if (!cache || needed > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp)            /* allocation failure – return 0 as a safe fallback */
            return 0;
        cache = tmp;
        if (needed > old_size)
            for (size_t i = old_size; i < needed; ++i)
                cache[i] = 0;   /* initialise new cells to zero */
        cache_size = needed;
    }

    /* Initialise base values if they are still unset (value 0) */
    if (cache_size >= 1 && cache[0] == 0) cache[0] = 113;          /* a(2) */
    if (cache_size >= 2 && cache[1] == 0) cache[1] = 765;          /* a(3) */
    if (cache_size >= 3 && cache[2] == 0) cache[2] = 5234;         /* a(4) */
    if (cache_size >= 4 && cache[3] == 0) cache[3] = 35865;        /* a(5) */

    /* Compute missing terms using the recurrence */
    for (size_t i = 4; i < needed; ++i) {
        cache[i] = cache[i-1] * 8 - cache[i-2] * 8 + cache[i-3];
    }

    return cache[n - OFFSET];
}
