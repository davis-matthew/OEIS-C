/*
 * A237244  Number of (n+1)X(3+1) 0..2 arrays with the maximum plus the upper
 *          median minus the lower median minus the minimum of every 2X2
 *          subblock differing from its horizontal and vertical neighbors by
 *          exactly one.
 * https://oeis.org/A237244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11576*a(n-4) - 33493888*a(n-8) for n>11 initial values:
 *            a(1)=1488, a(2)=12600, a(3)=108652, a(4)=974864, a(5)=8433384,
 *            a(6)=72877504, a(7)=629104640, a(8)=5642567936,
 *            a(9)=48822536832, a(10)=421991535616, a(11)=3643475019776,
 *            a(12)=32666380795904
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Persistent cache of computed terms. */
    static seq_t *cache = NULL;
    static idx_t cap = 0;
    static idx_t len = 0;   /* number of usable indices, i.e. cache[0..len-1] exist */

    if (len == 0) {
        /* Initialise cache with the first 12 known terms. */
        cap = 13;              /* indices 0..12 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* out‑of‑memory fallback */

        cache[0] = (seq_t)0;        /* unused */
        cache[1] = (seq_t)1488;
        cache[2] = (seq_t)12600;
        cache[3] = (seq_t)108652;
        cache[4] = (seq_t)974864;
        cache[5] = (seq_t)8433384;
        cache[6] = (seq_t)72877504;
        cache[7] = (seq_t)629104640;
        cache[8] = (seq_t)5642567936;
        cache[9] = (seq_t)48822536832;
        cache[10] = (seq_t)421991535616;
        cache[11] = (seq_t)3643475019776;
        cache[12] = (seq_t)32666380795904;
        len = 13;
    }

    if (n >= len) {
        /* Extend cache to accommodate n. */
        idx_t new_cap = (cap * 2 > n + 1) ? cap * 2 : n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* out‑of‑memory fallback */
        cache = new_cache;
        cap = new_cap;

        const seq_t coeff1 = (seq_t)11576;
        const seq_t coeff2 = (seq_t)33493888;

        for (idx_t i = len; i <= n; ++i) {
            /* Recurrence: a(n) = coeff1*a(n-4) - coeff2*a(n-8) */
            seq_t t1 = coeff1 * cache[i - 4];
            seq_t t2 = coeff2 * cache[i - 8];
            cache[i] = t1 - t2;
        }
        len = n + 1;
    }

    return cache[n];
}
