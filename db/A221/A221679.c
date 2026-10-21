/*
 * A221679  Number of 0..4 arrays of length n with each element differing from
 *          at least one neighbor by 1 or less, starting with 0.
 * https://oeis.org/A221679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2) + 6*a(n-4) + 4*a(n-5) + 4*a(n-6),
 *            initial values: a(1)=0, a(2)=2, a(3)=5, a(4)=26, a(5)=100,
 *            a(6)=418
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t max_n  = 0;

    if (n == 0)          /* offset starts at 1, but guard against 0 */
        return 0;

    if (n <= max_n)
        return cache[n];

    /* grow cache to hold up to n */
    cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!cache)          /* allocation failed; signal failure with 0 */
        return 0;

    /* fill new entries */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        switch (i) {
            case 1: cache[i] = 0;           break;
            case 2: cache[i] = 2;           break;
            case 3: cache[i] = 5;           break;
            case 4: cache[i] = 26;          break;
            case 5: cache[i] = 100;         break;
            case 6: cache[i] = 418;         break;
            default:
                cache[i] = 3 * cache[i-1]
                         + 4 * cache[i-2]
                         + 6 * cache[i-4]
                         + 4 * cache[i-5]
                         + 4 * cache[i-6];
        }
    }

    max_n = n;
    return cache[n];
}
