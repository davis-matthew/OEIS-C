/*
 * A001144  An exponential function on partitions (next term is 2^512).
 * https://oeis.org/A001144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^a(n-2) for n >= 7, a(1) = 1, a(2) = 2, a(3) = 3, a(4) =
 *            4, a(5) = 3^2 = 9, a(6) = 3^3 = 27
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   a001144_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001144_pow2.h"

seq_t a(idx_t n) {
    if (n <= 4) return (seq_t)n;
    if (n == 5) return (seq_t)3 * 3;
    if (n == 6) return (seq_t)3 * 3 * 3;
    seq_t e = a(n - 2);
    return a001144_pow2((idx_t)(unsigned long long)e);
}
