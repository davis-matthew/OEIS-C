/*
 * A172316  7th column of the array A172119.
 * https://oeis.org/A172316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+7) = 2*a(n+6) - a(n); a(0)=1, a(1)=2, a(2)=4, a(3)=8,
 *            a(4)=16, a(5)=32, a(6)=64
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of values currently stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* expand cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = n + 1;
        if (new_cap < 16) new_cap = 16;
        else if (new_cap < cache_cap * 2) new_cap = cache_cap * 2;
        seq_t *tmp;
        if (cache == NULL)
            tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        else
            tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* memory allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 7) {
            /* base cases */
            switch (i) {
                case 0: cache[i] = (seq_t)1;  break;
                case 1: cache[i] = (seq_t)2;  break;
                case 2: cache[i] = (seq_t)4;  break;
                case 3: cache[i] = (seq_t)8;  break;
                case 4: cache[i] = (seq_t)16; break;
                case 5: cache[i] = (seq_t)32; break;
                case 6: cache[i] = (seq_t)64; break;
            }
        } else {
            /* recurrence: a(i) = 2*a(i-1) - a(i-7) */
            cache[i] = 2 * cache[i - 1] - cache[i - 7];
        }
    }
    cache_len = n + 1;
    return cache[n];
}
