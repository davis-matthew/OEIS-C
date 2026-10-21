/*
 * A208559  Number of 7 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A208559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 31*a(n-2) + 12*a(n-3) – 144*a(n-4); initial
 *            values: a(1) = 20, a(2) = 400, a(3) = 1260, a(4) = 11020
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache the sequence. cache[0] is unused to keep indices 1‑based. */
    static seq_t *cache = NULL;
    static idx_t max_n = 0;

    if (n < 1)
        return 0;                     /* undefined for n < 1, return 0 */

    if (n <= max_n)                    /* already computed */
        return cache[n];

    /* Resize cache to hold indices up to n (including index 0). */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp)
        abort();                       /* allocation failed */
    cache = tmp;

    /* Compute the missing terms in increasing order. */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        if (i == 1)
            cache[i] = 20;
        else if (i == 2)
            cache[i] = 400;
        else if (i == 3)
            cache[i] = 1260;
        else if (i == 4)
            cache[i] = 11020;
        else
            cache[i] = cache[i - 1]
                     + 31 * cache[i - 2]
                     + 12 * cache[i - 3]
                     - 144 * cache[i - 4];
    }

    max_n = n;
    return cache[n];
}
