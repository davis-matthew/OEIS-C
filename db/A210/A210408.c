/*
 * A210408  Number of (n+1)X4 0..3 arrays with every 2X2 subblock having three
 *          or four distinct values, and new values 0..3 introduced in row
 *          major order.
 * https://oeis.org/A210408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 81*a(n-1)+1151*a(n-2)-13041*a(n-3)-117154*a(n-4)
                     +478740*a(n-5)+445664*a(n-6)-2873152*a(n-7)
                     +2422784*a(n-8)-196608*a(n-9);
   Initial values: a(1)=864, a(2)=80496, a(3)=7380816,
   a(4)=677993552, a(5)=62266883792, a(6)=5718722907600,
   a(7)=525218223843536, a(8)=48237041019306960,
   a(9)=4430181461077005264. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t allocated = 0;          /* number of slots allocated (including 0) */
    static idx_t  max_computed = 0;       /* highest n for which cache[..] is valid */

    /* --- initialise base values up to 9 --- */
    if (max_computed < 9) {
        /* ensure space for indices 0..9 (10 slots) */
        if (allocated < 10) {
            seq_t *tmp = (seq_t*)realloc(cache, 10 * sizeof(seq_t));
            if (!tmp) return 0;          /* out of memory – return zero */
            cache = tmp;
            for (size_t i = allocated; i < 10; ++i) cache[i] = 0;
            allocated = 10;
        }

        static const seq_t base[10] = {
            0ULL,                      /* unused: cache[0] */
            864ULL,
            80496ULL,
            7380816ULL,
            677993552ULL,
            62266883792ULL,
            5718722907600ULL,
            525218223843536ULL,
            48237041019306960ULL,
            4430181461077005264ULL
        };

        for (idx_t i = 1; i <= 9; ++i) cache[i] = base[i];
        max_computed = 9;
    }

    /* If requested index already computed, return it */
    if (n <= max_computed) return cache[n];

    /* --- ensure cache large enough for n --- */
    size_t need = (size_t)n + 1;          /* need indices 0..n */
    if (need > allocated) {
        seq_t *tmp = (seq_t*)realloc(cache, need * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        for (size_t i = allocated; i < need; ++i) cache[i] = 0;
        allocated = need;
    }

    /* --- compute missing terms using the recurrence --- */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t s_pos = cache[i - 1] * (seq_t)81
                    + cache[i - 2] * (seq_t)1151
                    + cache[i - 5] * (seq_t)478740
                    + cache[i - 6] * (seq_t)445664
                    + cache[i - 8] * (seq_t)2422784;

        seq_t s_neg = cache[i - 3] * (seq_t)13041
                    + cache[i - 4] * (seq_t)117154
                    + cache[i - 7] * (seq_t)2873152
                    + cache[i - 9] * (seq_t)196608;

        cache[i] = s_pos - s_neg;
    }
    max_computed = n;
    return cache[n];
}
