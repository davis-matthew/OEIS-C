/*
 * A109910  a(n) = 9's complement of digit reversal of n.
 * https://oeis.org/A109910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, look
 * Formula:   a(n) = 9's complement of digit reversal of n
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Step 1: reverse the decimal digits of n */
    seq_t rev = 0;
    idx_t temp = n;
    do {
        rev = rev * (seq_t)10 + (seq_t)(temp % 10);
        temp /= 10;
    } while (temp);

    /* Step 2: determine the number of digits of the reversed number
       (i.e., length after stripping leading zeros). */
    idx_t len = 0;
    if (rev == 0) {
        len = 1;
    } else {
        seq_t tmp = rev;
        while (tmp) {
            len++;
            tmp /= 10;
        }
    }

    /* Step 3: compute 10^len */
    seq_t pow10 = 1;
    for (idx_t i = 0; i < len; ++i) {
        pow10 *= (seq_t)10;
    }

    /* Step 4: 9's complement = (10^len - 1) - rev */
    seq_t nine_complement = pow10 - (seq_t)1;
    return nine_complement - rev;
}
