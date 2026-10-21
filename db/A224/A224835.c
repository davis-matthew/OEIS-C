/*
 * A224835  Sum of the cubes of the number of divisors function for those
 *          divisors of n that are less than or equal to the cube root of n.
 * https://oeis.org/A224835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{d|n,\, d \le n^{1/3}} \tau(d)^3
 * Range:     u8 n <= 1079, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_count_u64.h"
#include "../utils/int_cube_root.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t cube = int_cube_root(n);
    for (idx_t d = 1; d <= cube; ++d) {
        if (n % d == 0) {
            seq_t t = divisor_count_u64(d);
            sum += t * t * t;
        }
    }
    return sum;
}
