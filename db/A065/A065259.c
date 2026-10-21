/*
 * A065259  A057114 conjugated with A059893, inverse of A065260.
 * https://oeis.org/A065259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-4) - a(n-8) for n>8, with initial values a(1)=3,
 *            a(2)=1, a(3)=7, a(4)=2, a(5)=11, a(6)=5, a(7)=15, a(8)=4
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n == 0)          /* undefined for n=0, return 0 as sentinel */
        return (seq_t)0;

    if (cache_len < n) {
        idx_t new_len = n;
        if (new_len < 16) new_len = 16;                 /* small initial size */

        cache = (seq_t *)realloc(cache, (new_len + 1) * sizeof(seq_t));

        /* fill missing entries */
        for (idx_t i = cache_len + 1; i <= new_len; ++i) {
            if (i == 1)                  cache[i] = (seq_t)3;
            else if (i == 2)             cache[i] = (seq_t)1;
            else if (i == 3)             cache[i] = (seq_t)7;
            else if (i == 4)             cache[i] = (seq_t)2;
            else if (i == 5)             cache[i] = (seq_t)11;
            else if (i == 6)             cache[i] = (seq_t)5;
            else if (i == 7)             cache[i] = (seq_t)15;
            else if (i == 8)             cache[i] = (seq_t)4;
            else                         /* i > 8 : recurrence */
                cache[i] = (cache[i-4] + cache[i-4]) - cache[i-8];
        }
        cache_len = new_len;
    }
    return cache[n];
}
