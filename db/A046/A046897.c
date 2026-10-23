/*
 * A046897  Sum of divisors of n that are not divisible by 4.
 * https://oeis.org/A046897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sum_{d|n, 4∤d} d Computes the sum of all positive
 *            divisors of n that are not multiples of 4
 * Range:     u8 n <= 125, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            if (i % 4ull != 0ull)
                sum += (seq_t)i;
            idx_t j = n / i;
            if (j != i && j % 4ull != 0ull)
                sum += (seq_t)j;
        }
    }
    return sum;
}
