/*
 * A004428  Geometric mean of digits of n (rounded down).
 * https://oeis.org/A004428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor((product of decimal digits of n)^(1/k)), k = number
 *            of digits of n; a(0) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   power_leq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/power_leq.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    idx_t t = n;
    int k = 0;
    uint64_t prod = 1;
    while (t > 0) {
        uint64_t d = (uint64_t)(t % 10);
        if (d == 0) return (seq_t)0;
        prod *= d;
        k++;
        t /= 10;
    }
    for (uint64_t x = 9; x > 0; x--) {
        if (power_leq(x, k, prod)) return (seq_t)(unsigned)x;
    }
    return (seq_t)0;
}
