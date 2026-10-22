/*
 * A004721  Delete all 2's from the sequence of nonnegative integers.
 * https://oeis.org/A004721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = k with every digit 2 removed (the remaining digits read
 *            as a decimal number), where k is the n-th nonnegative integer
 *            (from 0) that has at least one digit other than 2
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t k = (uint64_t)n;
    uint64_t rep;
    for (rep = 2; rep <= k; rep = rep * 10 + 2)
        k++;
    seq_t res = 0, place = 1;
    if (k == 0) return res;
    while (k > 0) {
        unsigned d = (unsigned)(k % 10);
        k /= 10;
        if (d != 2) {
            res += place * d;
            place *= 10;
        }
    }
    return res;
}
