/*
 * A026062  a(n) = (d(n)-r(n))/5, where d = A026060 and r is the periodic
 *          sequence with fundamental period (0,0,1,4,0).
 * https://oeis.org/A026062
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A026062
 *
 *  a(n) = (d(n)-r(n))/5
 *
 *  It satisfies the linear recurrence
 *      a(n) = 3 a(n-1) - 3 a(n-2) + a(n-3)
 *             + a(n-5) - 3 a(n-6) + 3 a(n-7) - a(n-8)
 *  for n ≥ 13, with initial values
 *      a(5)=9, a(6)=16, a(7)=25, a(8)=36,
 *      a(9)=51, a(10)=68, a(11)=88, a(12)=111.
 *
 *  The function works for any seq_t and idx_t provided by the build.
 *  It caches computed values so subsequent calls are O(1) after
 *  the first collective computation up to that n.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached values of a(n)      */
static idx_t   cache_size = 0;   /* number of cached entries   */

static const seq_t init_vals[13] = {
    0, 0, 0, 0, 0,          /* indices 0..4 are unused   */
    9, 16, 25, 36, 51, 68, 88, 111   /* a(5)..a(12)               */
};

seq_t a(idx_t n)
{
    if (n < 5)          /* values below the offset are irrelevant */
        return 0;

    /* initialise cache on first call */
    if (cache_size == 0) {
        cache_size = 13;                     /* store indices 0..12   */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = init_vals[i];
    }

    /* compute missing entries up to n */
    if (n >= cache_size) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        for (idx_t i = cache_size; i <= n; ++i) {
            /* recurrence for i >= 13, the formula works for i=13..n */
            seq_t val = 3 * cache[i - 1];           /* 3 a(i-1) */
            val -= 3 * cache[i - 2];               /* -3 a(i-2) */
            val += cache[i - 3];                   /* +   a(i-3) */
            val += cache[i - 5];                   /* +   a(i-5) */
            val -= 3 * cache[i - 6];               /* -3 a(i-6) */
            val += 3 * cache[i - 7];               /* +3 a(i-7) */
            val -= cache[i - 8];                   /* -   a(i-8) */
            cache[i] = val;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
