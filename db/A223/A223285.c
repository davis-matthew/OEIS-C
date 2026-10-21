/*
 * A223285  Rolling icosahedron face footprints: number of 4 X n 0..19 arrays
 *          starting with 0 where 0..19 label faces of an icosahedron and
 *          every array movement to a horizontal, diagonal or antidiagonal
 *          neighbor moves across an icosahedral edge.
 * https://oeis.org/A223285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            5*a(n-1)+92*a(n-2)-56*a(n-3)-920*a(n-4)+192*a(n-5)+1152*a(n-6)
 *            for n>7 Initial values: a(1)=8000 , a(2)=375 , a(3)=8295 ,
 *            a(4)=72279 , a(5)=1024071 , a(6)=10979127 , a(7)=137621799
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation. Index 0 is unused. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size (indexable up to cache_cap-1) */
    static size_t cache_size = 0;  /* largest n for which cache[n] is defined */

    /* Initial values for n = 1..7 */
    static const seq_t init[8] = {
        0ULL,          /* dummy 0th entry */
        8000ULL,
        375ULL,
        8295ULL,
        72279ULL,
        1024071ULL,
        10979127ULL,
        137621799ULL
    };

    /* Convert idx_t to size_t for array indexing */
    size_t idx = (size_t)n;
    if (idx == 0) return 0;  /* undefined for n < 1, but return 0 */

    /* Grow the cache if necessary */
    if (cache_size < idx)
    {
        /* Ensure capacity for indices up to idx (inclusive) */
        if (idx >= cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap <= idx) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { /* allocation failed, abort */
                exit(EXIT_FAILURE);
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Initialize new entries */
        for (size_t i = cache_size + 1; i <= idx; ++i)
        {
            if (i < 8) {
                cache[i] = init[i];
            } else {
                /* Recurrence using only unsigned arithmetic.
                   term_pos = 5*a(n-1)+92*a(n-2)+192*a(n-5)+1152*a(n-6)
                   term_neg = 56*a(n-3)+920*a(n-4)
                   a(n) = term_pos - term_neg
                */
                seq_t term_pos =
                    (seq_t)5   * cache[i-1] +
                    (seq_t)92  * cache[i-2] +
                    (seq_t)192 * cache[i-5] +
                    (seq_t)1152* cache[i-6];

                seq_t term_neg =
                    (seq_t)56  * cache[i-3] +
                    (seq_t)920 * cache[i-4];

                cache[i] = term_pos - term_neg;
            }
        }
        cache_size = idx;
    }

    return cache[idx];
}
