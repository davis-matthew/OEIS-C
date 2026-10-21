/*
 * A073139  Difference between the largest and smallest number having in
 *          binary representation the same number of 0's and 1's as n.
 * https://oeis.org/A073139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2^(o-1))-1) * (2^z - 1), where * o = number of 1-bits
 *            in binary representation of n (with leading 1) * z = number of
 *            0-bits in that representation. * This difference is between the
 *            largest and smallest numbers that * have exactly the same counts
 *            of 0's and 1's as n
 * Range:     u8 n <= 514, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0)
        return 0;  /* by definition a(0) = 0 */

    idx_t ones = 0;
    idx_t len = 0;
    idx_t t = n;
    while (t) {
        len++;
        ones += t & 1;
        t >>= 1;
    }
    idx_t zeros = len - ones;

    /* (2^(o-1))-1 */
    seq_t part1 = ((seq_t)1 << (ones - 1)) - 1;
    /* 2^z - 1 */
    seq_t part2 = ((seq_t)1 << zeros) - 1;

    return part1 * part2;
}
