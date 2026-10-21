/*
 * A062028  a(n) = n + sum of the digits of n.
 * https://oeis.org/A062028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n + digit_sum(n) (sum of decimal digits of n)
 * Range:     u8 n <= 244, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for uint64_t if needed */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t t = n;
    while (t != 0) {
        sum += t % 10; /* t%10 is small, added to seq_t sum */
        t /= 10;
    }
    return (seq_t)n + sum; /* cast index to seq_t before addition */
}
