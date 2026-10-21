/*
 * A233675  Number of (n+1) X (1+1) 0..3 arrays with every 2 X 2 subblock
 *          having the sum of the squares of the edge differences equal to 6.
 * https://oeis.org/A233675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 12*a(n-2) - 4*a(n-3) - 16*a(n-4) * with a(1)=48,
 *            a(2)=184, a(3)=648, a(4)=2440
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 32, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t computed = 0;

    if (n <= computed)
        return cache[n - 1];

    size_t new_cap = (size_t)n;
    if (cache == NULL)
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    else
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));

    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1)
            val = (seq_t)48;
        else if (i == 2)
            val = (seq_t)184;
        else if (i == 3)
            val = (seq_t)648;
        else if (i == 4)
            val = (seq_t)2440;
        else
            val = cache[i - 2]          /* a(i-1) */
                + 12 * cache[i - 3]    /* 12*a(i-2) */
                - 4 * cache[i - 4]     /* -4*a(i-3) */
                - 16 * cache[i - 5];   /* -16*a(i-4) */
        cache[i - 1] = val;
    }

    computed = n;
    return cache[n - 1];
}
