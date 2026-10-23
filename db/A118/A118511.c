/*
 * A118511  Define sequence S_n by: initial term = n, reverse digits and add 1
 *          to get next term. It is conjectured that S_n always reaches a
 *          cycle of length 9, as in A117230. Sequence gives number of steps
 *          for S_n to reach the cycle.
 * https://oeis.org/A118511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A118511: number of steps for the map T(x) = reverse(x) + 1
   until x enters the cycle {2,3,4,5,6,7,8,9,10}. */
#include "../utils/reverse_digits.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1; safe guard */
        return 0;

    seq_t cur   = (seq_t)n;   /* starting term of the sequence S_n   */
    seq_t steps = 0;          /* count of performed iterations     */

    /* iterate until the current value is within the 9‑term cycle */
    while (cur < 2 || cur > 10) {
        cur    = reverse_digits(cur) + 1;
        steps++;
    }

    return steps;
}
