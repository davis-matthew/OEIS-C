/*
 * A223200  Rolling cube footprints: number of nX6 0..5 arrays starting with 0
 *          where 0..5 label faces of a cube and every array movement to a
 *          horizontal or vertical neighbor moves across a corresponding cube
 *          edge.
 * https://oeis.org/A223200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1920*a(n-1) - 1191936*a(n-2) + 335020032*a(n-3) -
 *            48670703616*a(n-4) + 3778497478656*a(n-5) -
 *            147884313935872*a(n-6) + 2269391999729664*a(n-7) with a(1)=1024,
 *            a(2)=995328, a(3)=1020002304, a(4)=1061444124672,
 *            a(5)=1110327429169152, a(6)=1163614255186968576,
 *            a(7)=1220273386789986631680
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"   /* allows seq_from_str() */

seq_t a(idx_t n)
{
    /* Static cache to store computed values.  cache[1] .. cache[7] are the
       known base values.  cache[0] is unused (set to 0). */
    static seq_t *cache          = NULL;      /* array of seq_t */
    static size_t cache_size    = 0;          /* allocated length */
    static idx_t  computed_up_to= 0;          /* highest n computed */

    if (!cache) {
        /* Allocate room for indices 0..7 initially. */
        cache_size = 8;                     /* indices 0 .. 7 inclusive */
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failure */

        cache[0] = (seq_t)0;                /* unused */

        /* Base values (provided explicitly).  All fit in 64 bits
           except a(7), for which we use seq_from_str. */
        cache[1] = (seq_t)1024;
        cache[2] = (seq_t)995328;
        cache[3] = (seq_t)1020002304;
        cache[4] = (seq_t)1061444124672ULL;
        cache[5] = (seq_t)1110327429169152ULL;
        cache[6] = (seq_t)1163614255186968576ULL;
        cache[7] = seq_from_str("1220273386789986631680");

        computed_up_to = 7;
    }

    /* Ensure the cache is large enough. */
    if ((idx_t)cache_size <= n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* realloc failure */
        cache     = tmp;
        cache_size = new_size;
    }

    /* Return early if the value has already been computed. */
    if (n <= computed_up_to)
        return cache[(size_t)n];

    /* Recurrence coefficients (as seq_t constants). */
    static const seq_t c0 = (seq_t)1920;
    static const seq_t c1 = (seq_t)1191936;
    static const seq_t c2 = (seq_t)335020032;
    static const seq_t c3 = (seq_t)48670703616ULL;
    static const seq_t c4 = (seq_t)3778497478656ULL;
    static const seq_t c5 = (seq_t)147884313935872ULL;
    static const seq_t c6 = (seq_t)2269391999729664ULL;

    /* Compute values from computed_up_to+1 up to n. */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        /* The recurrence uses a(i-1)..a(i-7).  For i <= 7
           the terms a(i-7) etc. are zero or unused, but the loop
           starts at i=8, so all references are valid. */
        seq_t val = c0 * cache[(size_t)(i-1)]
                  - c1 * cache[(size_t)(i-2)]
                  + c2 * cache[(size_t)(i-3)]
                  - c3 * cache[(size_t)(i-4)]
                  + c4 * cache[(size_t)(i-5)]
                  - c5 * cache[(size_t)(i-6)]
                  + c6 * cache[(size_t)(i-7)];
        cache[(size_t)i] = val;
    }

    computed_up_to = n;
    return cache[(size_t)n];
}
