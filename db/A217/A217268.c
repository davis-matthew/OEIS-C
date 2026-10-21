/*
 * A217268  Number of n X 3 arrays of occupancy after each element moves to
 *          some horizontal or vertical neighbor.
 * https://oeis.org/A217268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24*a(n-1) - 648*a(n-3) + 729*a(n-4) for n>5 Initial
 *            values: a(1)=2, a(2)=64, a(3)=1472, a(4)=33856, a(5)=772800
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t  computed = 0;

    if (n <= 0) return 0;           /* undefined for n < 1 */

    /* Resize cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap ? cap * 2 : 4;
        if (newcap < (size_t)n + 1) newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp && newcap != 0) return 0;  /* allocation failure */
        cache = tmp;
        cap = newcap;
    }

    /* Compute values up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 1) cache[1] = (seq_t)2;
        else if (i == 2) cache[2] = (seq_t)64;
        else if (i == 3) cache[3] = (seq_t)1472;
        else if (i == 4) cache[4] = (seq_t)33856;
        else if (i == 5) cache[5] = (seq_t)772800;
        else
            cache[i] = 24 * cache[i-1]
                       - 648 * cache[i-3]
                       + 729 * cache[i-4];
    }
    computed = n;
    return cache[n];
}
