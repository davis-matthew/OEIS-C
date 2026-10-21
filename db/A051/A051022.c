/*
 * A051022  Interpolate 0's between each pair of digits of n.
 * https://oeis.org/A051022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = number obtained by inserting a zero between each adjacent
 *            * decimal digit of n. * Recurrence: a(n) = n (if n < 10) * a(n)
 *            = a(floor(n/10))*100 + (n % 10) (otherwise) * This
 *            implementation works for all n ≥ 0 and uses only integer *
 *            arithmetic on types seq_t and idx_t
 * Range:     u8 n <= 29, u16 n <= 699, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    /* find the highest power of 10 not exceeding n */
    idx_t pow10 = 1;
    while (pow10 <= n/10) {
        pow10 *= 10;
    }

    idx_t temp = n;
    while (pow10 > 0) {
        idx_t digit = temp / pow10;
        temp %= pow10;
        pow10 /= 10;
        result = result * 100 + (seq_t)digit;
    }

    return result;
}
