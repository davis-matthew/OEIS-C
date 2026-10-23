/*
 * A195568  Denominators a(n) of Pythagorean approximations b(n)/a(n) to 7/4.
 * https://oeis.org/A195568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=3, a(2)=8, a(3)=120, a(4)=637, a(5)=2176,
   a(6)=30848, a(7)=164483, a(8)=561288, a(9)=7958776;
   for n ≥10: a(n) = 257*a(n-3) + 257*a(n-6) - a(n-9)   (derived from the G.F.) */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* cache of computed terms (1‑indexed mapping to array[0]) */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;
    static idx_t size  = 0; /* number of entries stored */

    if (n == 0) return 0;                 /* undefined for n=0, return 0 */

    /* enlarge cache if necessary */
    if (n > cap) {
        idx_t new_cap = cap ? cap << 1 : 64;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);                /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* first nine terms are known */
    static const seq_t init[9] = {
        3ULL,8ULL,120ULL,637ULL,2176ULL,30848ULL,
        164483ULL,561288ULL,7958776ULL
    };

    /* compute missing terms */
    for (idx_t i = size + 1; i <= n; ++i) {
        if (i <= 9) {
            cache[i - 1] = init[i - 1];
        } else {
            /* a(i) = 257*a(i-3) + 257*a(i-6) – a(i-9) */
            seq_t v = 257 * cache[i - 4]       /* a(i-3) */
                   + 257 * cache[i - 7]         /* a(i-6) */
                   -  cache[i - 10];            /* a(i-9) */
            cache[i - 1] = v;
        }
    }
    size = n;

    return cache[n - 1];
}
