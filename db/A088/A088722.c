/*
 * A088722  Number of divisors d>1 of n such that d+1 also divides n.
 * https://oeis.org/A088722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{d|n, d>1, (d+1)|n} 1 (offset 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of divisors d>1 of n such that d+1 also divides n. */
#include <stdbool.h>
#include <stdint.h>

/* The types `seq_t` and `idx_t` are supplied by the build environment. */
seq_t a(idx_t n)
{
    /* n is at least 1 according to the offset, but guard against 0 just in case. */
    if (n < 2) return 0;

    seq_t count = 0;
    idx_t i;
    for (i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            /* Check the smaller divisor d1. */
            if (d1 > 1 && n % (d1 + 1) == 0) {
                ++count;
            }

            /* Check the complementary divisor d2, avoid double counting. */
            if (d2 != d1 && d2 > 1 && n % (d2 + 1) == 0) {
                ++count;
            }
        }
    }
    return count;
}
