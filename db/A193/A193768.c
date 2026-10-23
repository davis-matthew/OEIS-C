/*
 * A193768  The domination number of the 4 X n board.
 * https://oeis.org/A193768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n except for n = 1, 2, 3, 5, 6 or 9 where a(n) = n + 1
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* out of range, but not expected */

    seq_t val = (seq_t)n;
    switch (n)             /* add 1 for the exceptional indices */
    {
        case 1:
        case 2:
        case 3:
        case 5:
        case 6:
        case 9:
            val += 1;
            break;
        default:
            break;
    }
    return val;
}
