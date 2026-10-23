/*
 * A039134  Numbers whose base-10 representation has the same number of 2's
 *          and 9's.
 * https://oeis.org/A039134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   equal_2_9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A039134: numbers whose decimal representation has the same
 * number of 2's and 9's. This implementation searches sequentially
 * from 0 upward until the nth such number is found. */
#include <stdbool.h>
#include "../utils/equal_2_9.h"

seq_t a(idx_t n)
{
    seq_t cand = 0;
    idx_t count = 0;
    while (count < n)
    {
        if (equal_2_9(cand))
        {
            ++count;
            if (count == n)
                return cand;
        }
        cand = cand + 1;
    }
    /* Should never reach here for n >= 1 */
    return 0;
}
