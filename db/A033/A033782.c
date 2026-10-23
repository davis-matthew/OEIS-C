/*
 * A033782  Product t2(q^d); d | 23, where t2 = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of positive odd solutions to x^2 + 23 y^2 = 8(n +
 *            3)
 * Range:     u8 n <= 7, u16 n <= 1914, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t target = 8 * ((seq_t)n + 3);
    seq_t limit = floor_sqrt(target / 23);

    for (idx_t y = 1; y <= (idx_t)limit; y += 2) {
        seq_t y_sq  = (seq_t)y * (seq_t)y;
        seq_t sub   = 23 * y_sq;
        if (sub > target) break;

        seq_t cur   = target - sub;
        seq_t x     = floor_sqrt(cur);

        if (x * x == cur && (x & 1) == 1) {
            count++;
        }
    }

    return count;
}
