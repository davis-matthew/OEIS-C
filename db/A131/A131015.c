/*
 * A131015  Period 12: repeat 1, 1, 3, 2, 2, 1, 4, 4, 2, 3, 3, 4.
 * https://oeis.org/A131015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) - a(n-6) + a(n-7) (n >= 7) Initial values: a(0)=1,
 *            a(1)=1, a(2)=3, a(3)=2, a(4)=2, a(5)=1, a(6)=4
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap = 0;

    /* Initialize cache with the first 8 terms (0 through 7). */
    if (cap == 0) {
        cap = 8;
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache) return 0;             /* allocation failure */
        cache[0] = 1;  cache[1] = 1;  cache[2] = 3;
        cache[3] = 2;  cache[4] = 2;  cache[5] = 1;
        cache[6] = 4;  cache[7] = 4;
    }

    if (n < cap) return cache[n];

    /* Grow the cache if necessary. */
    idx_t newcap = n + 1;
    if (newcap > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failure */
        cache = tmp;
        cap = newcap;
    }

    /* Compute values up to n using the recurrence. */
    for (idx_t i = 7; i <= n; ++i) {
        int diff = (int)cache[i - 1] - (int)cache[i - 6];
        cache[i] = (seq_t)(diff + (int)cache[i - 7]);
    }

    return cache[n];
}
