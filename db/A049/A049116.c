/*
 * A049116  Repeatedly apply Euler phi to n; a(n) = highest power of 2 that is
 *          seen.
 * https://oeis.org/A049116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum power of 2 appearing in the iterated phi sequence
 *            starting at n. The function repeatedly applies Euler's totient
 *            function phi to n until reaching 1, keeping track of the largest
 *            power of two encountered
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/calc_phi.h"
#include "../utils/is_power_of_two.h"

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    seq_t max = 0;

    while (1)
    {
        if (is_power_of_two(m) && m > max)
            max = m;

        if (m == (seq_t)1)
            break;

        m = calc_phi(m);
    }

    return max;
}
