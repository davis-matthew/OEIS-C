/*
 * A007844  Least positive integer k for which 3^n divides k!.
 * https://oeis.org/A007844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k with v_3(k!) >= n, where v_3(k!) = Sum_{i>=1}
 *            floor(k/3^i); found by binary search on k in [1, 3n]
 * Range:     u8 n <= 125, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   exponent_of_3_in_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/exponent_of_3_in_factorial.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;

    seq_t target = (seq_t)n;
    seq_t left = 1, right = (seq_t)3 * target;

    while (left < right) {
        seq_t mid = left + (right - left) / 2;
        if (exponent_of_3_in_factorial(mid) < target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    return left;
}
