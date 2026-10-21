/*
 * A078757  Values of A028470(n)/A000045(n+1).
 * https://oeis.org/A078757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 25*a(n-2) + 11*a(n-3) - 47*a(n-4) - 11*a(n-5) +
 *            25*a(n-6) - a(n-7) - a(n-8) Initial terms (offset 1): a(1)=1,
 *            a(2)=17, a(3)=51, a(4)=449, a(5)=1853, a(6)=12853, a(7)=61557,
 *            a(8)=382024
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define _USE_MATH_DEFINES
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoization table */
    static idx_t cap = 0;         /* current allocated size (+1 for 0-index) */

    /* initialise with known first 8 terms */
    if (cache == NULL) {
        cap = 9; /* indices 0..8 */
        cache = (seq_t*)malloc(sizeof(seq_t)*cap);
        if (cache == NULL) return 0;            /* allocation failed */
        cache[0] = 0;                           /* unused; keeps indices natural */
        cache[1] = 1;
        cache[2] = 17;
        cache[3] = 51;
        cache[4] = 449;
        cache[5] = 1853;
        cache[6] = 12853;
        cache[7] = 61557;
        cache[8] = 382024;
    }

    /* for n == 0 (outside the defined offset) return 0 */
    if (n < 1) return 0;

    /* expand cache if needed and compute missing values */
    if (n >= cap) {
        idx_t newcap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t)*newcap);
        if (tmp == NULL) return 0; /* allocation failed */
        cache = tmp;
        for (idx_t i = cap; i <= n; ++i) {
            seq_t pos = cache[i-1];
            pos += (seq_t)25 * cache[i-2];
            pos += (seq_t)11 * cache[i-3];
            pos += (seq_t)25 * cache[i-6];

            seq_t neg = (seq_t)47 * cache[i-4];
            neg += (seq_t)11 * cache[i-5];
            neg += cache[i-7];
            neg += cache[i-8];

            cache[i] = pos - neg;
        }
        cap = newcap;
    }

    return cache[n];
}
