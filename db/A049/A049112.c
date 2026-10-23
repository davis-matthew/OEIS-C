/*
 * A049112  2-ranks of difference sets constructed from Glynn type I
 *          hyperovals.
 * https://oeis.org/A049112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+1) = a(n) + a(n-1) + a(n-2) + a(n-3) - 1, n >= 5; a(1)=1,
 *            a(2)=1, a(3)=3, a(4)=7, a(5)=13;
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 35, u64 n <= 68, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached values – cache[0] is unused to keep indices 1‑based */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;   /* highest index that has been computed */

    if (n == 0)
        return (seq_t)0;          /* value for undefined offset */

    /* Ensure the array is large enough for index n */
    size_t new_cap = (size_t)n + 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!cache)
        abort();                 /* out of memory */

    /* Initialise first terms if they have not yet been set */
    if (cached_len < 5) {
        idx_t start = cached_len + 1;
        for (idx_t i = start; i <= n && i <= 5; ++i) {
            switch (i) {
                case 1: cache[1] = (seq_t)1; break;
                case 2: cache[2] = (seq_t)1; break;
                case 3: cache[3] = (seq_t)3; break;
                case 4: cache[4] = (seq_t)7; break;
                case 5: cache[5] = (seq_t)13; break;
            }
        }
    }

    /* Compute any missing values up to n */
    for (idx_t i = cached_len + 1; i <= n; ++i) {
        if (i <= 5) {
            /* already initialised */
            continue;
        }
        /* recurrence: a(i) = a(i-1)+a(i-2)+a(i-3)+a(i-4)-1 */
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4] - (seq_t)1;
    }

    if (n > cached_len)
        cached_len = n;

    return cache[n];
}
