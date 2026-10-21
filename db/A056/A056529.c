/*
 * A056529  Number of iterations of sum of digits of square to reach 1, 9, 13
 *          or 16.
 * https://oeis.org/A056529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number of iterations of digit_sum(square) needed to reach
 *            one of {1, 9, 13, 16}. The initial value n is considered step 0;
 *            if n already equals a target, a(n)=0. Otherwise the operation y
 *            = digit_sum(y*y) is applied repeatedly until a target is reached
 * Range:     u8 n <= 16, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   digit_sum_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_sum_local.h"

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;
    seq_t steps = 0;

    /* Return immediately if the starting value is already a target. */
    if (cur == 1 || cur == 9 || cur == 13 || cur == 16)
        return steps;

    while (true) {
        /* Apply the transformation: square and sum of digits. */
        seq_t sq = cur * cur;
        cur = digit_sum_local(sq);
        ++steps;

        /* Check whether the new value reaches a target. */
        if (cur == 1 || cur == 9 || cur == 13 || cur == 16)
            return steps;
    }
}
