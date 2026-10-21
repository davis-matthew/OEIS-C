/*
 * A209506  Half the number of (n+1) X 4 0..2 arrays with every 2 X 2 subblock
 *          having two or four distinct clockwise edge differences.
 * https://oeis.org/A209506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 15*a(n-2) + 6*a(n-3) Initial values: a(1)=81,
 *            a(2)=561, a(3)=3906
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* 0‑based array, cache[0] unused */
    static size_t cap   = 0;         /* current allocated capacity */

    if (n == 0)
        return 0;                    /* undefined for n<1 */

    /* Ensure array is large enough */
    if ((size_t)n >= cap) {
        size_t newcap = cap ? cap << 1 : 16;
        while (newcap <= (size_t)n)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure */
        /* initialize new elements to 0 */
        for (size_t i = cap; i < newcap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cap   = newcap;
    }

    /* Base cases */
    if (n == 1) { cache[1] = (seq_t)81;   return cache[1]; }
    if (n == 2) { cache[2] = (seq_t)561;  return cache[2]; }
    if (n == 3) { cache[3] = (seq_t)3906; return cache[3]; }

    /* Compute values up to n if not already computed */
    for (idx_t i = 4; i <= n; ++i) {
        if (cache[i] == 0) {
            cache[i] = (seq_t)9 * cache[i-1]
                     - (seq_t)15 * cache[i-2]
                     + (seq_t)6  * cache[i-3];
        }
    }
    return cache[n];
}
