/*
 * A017840  Expansion of 1/(1-x^5-x^6-x^7-x^8-x^9).
 * https://oeis.org/A017840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-5)+a(n-6)+a(n-7)+a(n-8)+a(n-9) for n>8 Initial
 *            values: a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=1, a(6)=1,
 *            a(7)=1, a(8)=1
 * Range:     u8 n <= 31, u16 n <= 54, u32 n <= 101, u64 n <= 194,
 *            u128 n <= 380, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;      /* allocated capacity */
    static idx_t  last = (idx_t)-1;/* last computed index */

    /* initial allocation and seed base values */
    if (cache == NULL) {
        cap = 10;  /* enough for indices 0..9 */
        cache = (seq_t*)malloc(sizeof(seq_t) * cap);
        if (!cache) abort();          /* terminate on allocation failure */

        cache[0] = (seq_t)1;
        for (size_t i = 1; i <= 4; ++i) cache[i] = (seq_t)0;
        for (size_t i = 5; i <= 8; ++i) cache[i] = (seq_t)1;
        last = 8;
    }

    /* ensure capacity */
    if ((idx_t)n + 1 > (idx_t)cap) {
        size_t newcap = (size_t)n + 1;
        if (newcap < cap * 2) newcap = cap * 2;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * newcap);
        if (!tmp) abort();
        cache = tmp;
        cap = newcap;
    }

    /* extend computation up to n */
    for (idx_t i = last + 1; (idx_t)i <= n; ++i) {
        if (i <= 8) continue;  /* base values already set */
        cache[(size_t)i] =
            cache[(size_t)(i - 5)] +
            cache[(size_t)(i - 6)] +
            cache[(size_t)(i - 7)] +
            cache[(size_t)(i - 8)] +
            cache[(size_t)(i - 9)];
    }
    last = n;

    return cache[(size_t)n];
}
