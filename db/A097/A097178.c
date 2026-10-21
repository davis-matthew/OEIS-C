/*
 * A097178  Expansion of (1+10*x-101*x^2-900*x^3)/((1-100*x^2)*(1-101*x^2)).
 * https://oeis.org/A097178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0)=1, a(1)=10, a(2)=100, a(3)=1110,
 * a(n)=201*a(n-2)-10100*a(n-4)  for n>=4
 */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for already computed terms */
    static seq_t *cache = NULL;
    static idx_t sz = 0;   /* number of entries currently computed   */
    static idx_t cap = 0;  /* allocated size of cache (in seq_t units) */

    /* ensure we have enough storage */
    if (cap < n + 1) {
        idx_t newcap = cap ? cap * 2 : 16;
        while (newcap < n + 1) newcap *= 2;
        seq_t *newbuf = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (cache) {
            memcpy(newbuf, cache, sz * sizeof(seq_t));
            free(cache);
        }
        cache = newbuf;
        cap = newcap;
    }

    /* initialise the first four terms if not done yet */
    if (sz < 4) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
        cache[2] = (seq_t)100;
        cache[3] = (seq_t)1110;
        sz = 4;
    }

    /* compute terms up to n if not already present */
    if (n + 1 > sz) {
        for (idx_t i = sz; i <= n; ++i) {
            /* recurrence: a(i) = 201*a(i-2) - 10100*a(i-4) */
            seq_t t1 = cache[i - 2];
            seq_t t2 = cache[i - 4];
            seq_t prod1 = (seq_t)201 * t1;
            seq_t prod2 = (seq_t)10100 * t2;
            cache[i] = prod1 - prod2;   /* guaranteed positive by definition */
        }
        sz = n + 1;
    }

    return cache[n];
}
