/*
 * A118554  a(n) = 6*a(n-5) - a(n-10) + 98 with a(0)=0, a(1)=11, a(2)=35,
 *          a(3)=56, a(4)=104, a(5)=147, a(6)=204, a(7)=336, a(8)=455,
 *          a(9)=731.
 * https://oeis.org/A118554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-5) - a(n-10) + 98, a(0)=0, a(1)=11, a(2)=35,
 *            a(3)=56, a(4)=104, a(5)=147, a(6)=204, a(7)=336, a(8)=455,
 *            a(9)=731
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 53, u64 n <= 116, u128 n <= 241,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t capacity = 0;      /* allocated size of cache      */
    static idx_t up_to   = 0;        /* largest index already computed */
    static const seq_t base[10] = {
        (seq_t)0,  (seq_t)11, (seq_t)35, (seq_t)56, (seq_t)104,
        (seq_t)147,(seq_t)204,(seq_t)336,(seq_t)455,(seq_t)731
    };

    /* initialise cache on first call */
    if (capacity == 0) {
        capacity = 10;               /* start with space for first 10 terms */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        for (idx_t i = 0; i < 10; ++i)
            cache[i] = base[i];
        up_to = 9;
    }

    /* grow cache if needed */
    if ((size_t)(n + 1) > capacity) {
        size_t new_cap = capacity;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;                      /* exponential growth */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        capacity = new_cap;

        /* fill any newly allocated entries with default values */
        for (idx_t i = up_to + 1; i <= (idx_t)(new_cap - 1) && i < 10; ++i)
            cache[i] = base[i];
        if (up_to < 9)
            up_to = 9;
    }

    /* return already computed value */
    if (n <= up_to)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = up_to + 1; i <= n; ++i) {
        if (i < 10)
            cache[i] = base[i];
        else
            cache[i] = (seq_t)6 * cache[i - 5] - cache[i - 10] + (seq_t)98;
    }
    up_to = n;
    return cache[n];
}
