/*
 * A032086  Number of reversible strings with n beads of 3 colors. If more
 *          than 1 bead, not palindromic.
 * https://oeis.org/A032086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) - 9*a(n-3) (n > 4) with initial
 *            values a(1)=3, a(2)=3, a(3)=9, a(4)=36
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed values.
       Index 0 is unused; values are stored at positions 1..k. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of allocated slots (including index 0) */

    if (n == 0)          /* sequence is defined for n ≥ 1; 0 is out of range */
        return (seq_t)0;

    /* Grow the cache if the requested index is not yet present */
    if (n + 1 > cache_size) {          /* need indices up to n inclusive */
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {                     /* allocation failed – abort */
            return (seq_t)0;
        }
        cache = tmp;

        /* Initialise newly allocated slots */
        for (idx_t i = cache_size; i < new_len; ++i) {
            switch (i) {
                case 0:
                    cache[0] = (seq_t)0;          /* unused */
                    break;
                case 1:
                    cache[1] = (seq_t)3;
                    break;
                case 2:
                    cache[2] = (seq_t)3;
                    break;
                case 3:
                    cache[3] = (seq_t)9;
                    break;
                case 4:
                    cache[4] = (seq_t)36;
                    break;
                default:                          /* recurrence for i ≥ 5 */
                    cache[i] = 3 * cache[i - 1]
                            + 3 * cache[i - 2]
                            - 9 * cache[i - 3];
                    break;
            }
        }
        cache_size = new_len;
    }

    return cache[n];
}
