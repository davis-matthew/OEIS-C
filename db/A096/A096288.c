/*
 * A096288  Sum of digits of n in bases 2 and 3.
 * https://oeis.org/A096288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = popcount(n) + sum_of_digits_base3(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;

    /* Base‑2 popcount */
    idx_t t = n;
    while (t) {
        sum += (seq_t)(t & 1);
        t >>= 1;
    }

    /* Base‑3 digit sum */
    t = n;
    while (t) {
        sum += (seq_t)(t % 3);
        t /= 3;
    }

    return sum;
}
