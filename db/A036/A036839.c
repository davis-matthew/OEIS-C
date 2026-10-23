/*
 * A036839  RATS(n): Reverse Add Then Sort the digits.
 * https://oeis.org/A036839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, nice, easy, look
 * Formula:   a(n) = sortdigits( n + rev(n) ) (digits sorted in increasing
 *            order)
 * Range:     u8 n <= 113, u16 n <= 8575, u32 n <= 8575, u64 n <= 8575,
 *            u128 n <= 8575, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert index to sequence type */
    seq_t x = (seq_t)n;

    /* Reverse the digits of x */
    seq_t rev = 0;
    seq_t temp = x;
    while (temp > 0) {
        seq_t digit = temp % (seq_t)10;
        rev = rev * (seq_t)10 + digit;
        temp = temp / (seq_t)10;
    }

    /* Sum x and its reverse */
    seq_t sum = x + rev;

    /* Count the digits in the sum */
    unsigned short cnt[10] = {0};
    temp = sum;
    while (temp > 0) {
        unsigned short d = (unsigned short)(temp % (seq_t)10);
        cnt[d]++;
        temp = temp / (seq_t)10;
    }

    /* Reconstruct the number with digits sorted increasingly */
    seq_t result = 0;
    for (int d = 0; d < 10; ++d) {
        while (cnt[d]--) {
            result = result * (seq_t)10 + d;
        }
    }

    return result;
}
