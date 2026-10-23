/*
 * A107846  Number of duplicate digits of n.
 * https://oeis.org/A107846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = A055642(n) - A043537(n) (number of digits of n minus
 *            number of distinct decimal digits)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Count total digits and distinct digits of n in base 10. */
    idx_t temp = n;
    int total_digits = 0;
    uint16_t mask = 0;
    int distinct_digits = 0;

    /* Handle the special case n == 0: one digit '0' and one distinct digit. */
    if (temp == (idx_t)0) {
        return (seq_t)0; /* 1 - 1 = 0 */
    }

    while (temp > (idx_t)0) {
        int digit = (int)(temp % (idx_t)10);
        total_digits++;

        if (!(mask & (1u << digit))) {
            mask |= (1u << digit);
            distinct_digits++;
        }

        temp /= (idx_t)10;
    }

    return (seq_t)(total_digits - distinct_digits);
}
