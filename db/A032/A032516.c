/*
 * A032516  Sum of the integer part of 7/2-th roots of integers less than n.
 * https://oeis.org/A032516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{n} floor(k^(2/7))
 * Range:     u8 n <= 104, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_7th_root_of_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the integer part of the (7/2)-th root of k by testing successive m
 * until m^7 > k^2, so no floating point operations are used. */
#include <stdint.h>
#include "../utils/floor_7th_root_of_square.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        uint64_t root = floor_7th_root_of_square((uint64_t)k);
        sum += (seq_t)root;
    }
    return sum;
}
