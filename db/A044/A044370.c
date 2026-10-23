/*
 * A044370  Numbers n such that string 3,8 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has38
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth positive integer whose decimal representation contains
   the substring "38" while the preceding integer (n−1) does not. */

#include <stdbool.h>
#include "../utils/has38.h"

/* Return the nth term of the sequence (1‑based index).  The function
   scans through the integers, counting those that fulfill the
   described property, until the desired index is reached. */
seq_t a(idx_t n)
{
    if (n == 0)        /* undefined; return 0 for safety */
        return 0;

    idx_t cur = 1;            /* start at 1 */
    idx_t count = 0;          /* how many qualifying numbers seen */

    while (count < n) {
        int cur_has38 = has38(cur);
        int prev_has38 = (cur > 1) ? has38(cur - 1) : 0;
        if (cur_has38 && !prev_has38) {
            ++count;
            if (count == n)
                return (seq_t)cur;
        }
        ++cur;
    }

    /* Should never reach here for valid n */
    return 0;
}
