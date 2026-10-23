/*
 * A141022  a(n) = n mod ((sum of digits of n)+1).
 * https://oeis.org/A141022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = n mod ((sum of digits of n)+1)
 * Range:     u8 n <= 5080, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* compute the sum of decimal digits of n (n is uint64_t) */
    uint64_t tmp = n;
    unsigned sum = 0;
    while (tmp) {
        sum += tmp % 10;
        tmp /= 10;
    }

    /* divisor = sum_of_digits + 1 */
    seq_t div = (seq_t)sum + 1;

    /* return n mod div, using seq_t arithmetic */
    return (seq_t)n % div;
}
