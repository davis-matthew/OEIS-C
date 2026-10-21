/*
 * A062918  Sum of the digit reversals of the first n natural numbers.
 * https://oeis.org/A062918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Sum_{i=1}^{n} reverse(i)
 * Range:     u8 n <= 15, u16 n <= 225, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* reverse(i) is the decimal digit reversal of i (e.g. 12 → 21, 10 → 1) */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    __uint128_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        uint64_t x = (uint64_t)i;          /* idx_t is uint64_t or smaller */
        uint64_t rev = 0;
        while (x) {
            rev = rev * 10 + (x % 10);
            x /= 10;
        }
        sum += rev;
    }
    return (seq_t)sum;
}
