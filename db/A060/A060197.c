/*
 * A060197  Start at n, repeatedly apply pi(x) until reach 0; a(n) = number of
 *          steps to reach 0.
 * https://oeis.org/A060197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the iterated PrimePi sequence starting at n and
 *            ending with 0 (includes the starting term)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    seq_t count   = 1;          /* include the starting value n */
    seq_t current = (seq_t)n;   /* current element of the sequence */

    while (current > 0) {
        current = compute_pi((idx_t)current);
        count++;
    }

    return count;
}
