/*
 * A051531  Molien series for group H_{1,3}^{8} of order 2304.
 * https://oeis.org/A051531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0)=1, a(1)=1, a(2)=4, a(3)=15, a(4)=24, a(5)=44, a(6)=81,
 *            a(7)=115; a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) - 4*a(n-4) +
 *            2*a(n-5) - a(n-6) + 2*a(n-7) - a(n-8) for n >= 8
 * Range:     u8 n <= 9, u16 n <= 60, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap = 0;
    static idx_t len = 0;

    /* Initialize cache on first call */
    if (cap == 0) {
        cap = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache) abort();                /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)15;
        cache[4] = (seq_t)24;
        cache[5] = (seq_t)44;
        cache[6] = (seq_t)81;
        cache[7] = (seq_t)115;
        len  = 8;
    }

    if (n < len) return cache[n];

    /* Grow buffer if necessary */
    if (n + 1 > cap) {
        idx_t newcap = cap;
        while (newcap <= n) newcap <<= 1;   /* double */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
        if (!tmp) abort();                  /* allocation failure */
        cache = tmp;
        cap   = newcap;
    }

    /* Iterate forward, filling in missing terms */
    for (idx_t i = len; i <= n; ++i) {
        seq_t val = (seq_t)2 * cache[i-1]          - cache[i-2]
                  + (seq_t)2 * cache[i-3]          - (seq_t)4 * cache[i-4]
                  + (seq_t)2 * cache[i-5]          - cache[i-6]
                  + (seq_t)2 * cache[i-7]          - cache[i-8];
        cache[i] = val;
    }
    len = n + 1;
    return cache[n];
}
