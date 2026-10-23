/*
 * A037900  a(n) = (greatest base 6 digit of n) - (least base 6 digit of n).
 * https://oeis.org/A037900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (max digit of n in base 6) - (min digit of n in base 6)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t temp = n;
    seq_t maxDigit = 0;
    seq_t minDigit = 5; /* maximum possible digit in base 6 */

    while (temp > 0) {
        seq_t digit = (seq_t)(temp % 6);
        if (digit > maxDigit)
            maxDigit = digit;
        if (digit < minDigit)
            minDigit = digit;
        temp /= 6;
    }
    return maxDigit - minDigit;
}
