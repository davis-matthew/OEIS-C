/*
 * A017855  Expansion of 1/(1-x^6-x^7-x^8-x^9-x^10-x^11-x^12-x^13-x^14-x^15).
 * https://oeis.org/A017855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            a(n-6)+a(n-7)+a(n-8)+a(n-9)+a(n-10)+a(n-11)+a(n-12)+a(n-13)+a(n-14)+a(n-15)
 *            for n>14; a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0,
 *            a(6)=1, a(7)=1, a(8)=1, a(9)=1, a(10)=1, a(11)=1, a(12)=2,
 *            a(13)=3, a(14)=4, a(15)=5
 * Range:     u8 n <= 32, u16 n <= 54, u32 n <= 100, u64 n <= 192,
 *            u128 n <= 376, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* initialise first 16 terms once */
    if (cap == 0) {
        const seq_t init[16] = {
            (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0,
            (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1,
            (seq_t)2, (seq_t)3, (seq_t)4, (seq_t)5
        };
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* memory failure – unlikely in tests */
        for (size_t i = 0; i < cap; ++i)
            cache[i] = init[i];
    }

    /* grow cache if necessary */
    if ((idx_t)n >= cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* memory failure – unlikely in tests */
        cache = tmp;
        /* compute new entries */
        for (idx_t i = cap; i <= n; ++i) {
            if (i <= 15) continue;           /* already initialised */
            seq_t sum = 0;
            sum += cache[i - 6];
            sum += cache[i - 7];
            sum += cache[i - 8];
            sum += cache[i - 9];
            sum += cache[i -10];
            sum += cache[i -11];
            sum += cache[i -12];
            sum += cache[i -13];
            sum += cache[i -14];
            sum += cache[i -15];
            cache[i] = sum;
        }
        cap = new_cap;
    }
    return cache[n];
}
