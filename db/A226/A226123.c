/*
 * A226123  Number of terms of the form 2^k in Collatz(3x+1) trajectory of n.
 * https://oeis.org/A226123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of terms of the form 2^k in the Collatz trajectory
 *            of n
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_power_of_two.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t count = 0;

    for (;;) {
        if (is_power_of_two(x))
            count++;

        if (x == (seq_t)1)
            break;

        if ((x & (seq_t)1) == 0)
            x >>= 1;
        else
            x = 3 * x + 1;
    }

    return count;
}
