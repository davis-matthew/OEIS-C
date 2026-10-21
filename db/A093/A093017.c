/*
 * A093017  Luhn algorithm double-and-add sum of digits of n.
 * https://oeis.org/A093017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = sum of digits of n after applying the Luhn doubling
 *            algorithm, offset 0
 * Range:     u8 n <= 1145, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    bool double_next = false;
    seq_t tmp = (seq_t)n;          /* use seq_t for arithmetic with the index */
    while (tmp > 0) {
        int d = (int)(tmp % 10);
        if (double_next) {
            int dd = d * 2;
            if (dd > 9) dd -= 9;   /* Luhn rule: if doubled digit >9, subtract 9 */
            sum += (seq_t)dd;
        } else {
            sum += (seq_t)d;
        }
        double_next = !double_next;
        tmp /= 10;
    }
    return sum;
}
