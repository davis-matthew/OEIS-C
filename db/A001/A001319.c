/*
 * A001319  Number of (unordered) ways of making change for n cents using
 *          coins of 2, 5, 10, 20, 50 cents.
 * https://oeis.org/A001319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{50i+20j+10t <= n} c(n-50i-20j-10t), where c(r) = #{y
 *            >= 0 : 5y <= r, y == r (mod 2)} counts the partitions of r into
 *            parts 2 and 5 (coefficient of x^n in
 *            1/((1-x^2)(1-x^5)(1-x^10)(1-x^20)(1-x^50)))
 * Range:     u8 n <= 109, u16 n <= 589, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a001319_c25
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001319_c25.h"

seq_t a(idx_t n) {
    seq_t total = 0;
    for (uint64_t i = 0; 50 * i <= n; i++) {
        uint64_t r1 = n - 50 * i;
        for (uint64_t j = 0; 20 * j <= r1; j++) {
            uint64_t r2 = r1 - 20 * j;
            uint64_t s = 0;
            for (uint64_t t = 0; 10 * t <= r2; t++)
                s += a001319_c25(r2 - 10 * t);
            total += (seq_t)s;
        }
    }
    return total;
}
