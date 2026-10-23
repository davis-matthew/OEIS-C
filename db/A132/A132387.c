/*
 * A132387  a(n) = n with each digit d of n replaced by d mod 4.
 * https://oeis.org/A132387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = replace each decimal digit d of n by d mod 4
 * Range:     u8 n <= 299, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/*  Sequence A132387:
 *  a(n) = the decimal number obtained from n by replacing each digit d
 *  with d mod 4. Leading zeros of the resulting digit string are discarded.
 */
seq_t a(idx_t n)
{
    /* maximum number of decimal digits in a 64‑bit unsigned integer
       (since idx_t is uint64_t).  20 digits is sufficient for values
       up to 2^64-1.  The function works for n = 0 as a special case. */
    uint8_t digits[20];
    size_t len = 0;
    idx_t temp = n;

    if (temp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (temp != 0) {
            digits[len++] = (uint8_t)(temp % 10);
            temp /= 10;
        }
    }

    seq_t result = 0;
    for (size_t i = len; i > 0; --i) {
        uint8_t d = digits[i - 1];
        result = result * (seq_t)10 + (seq_t)(d % 4);
    }
    return result;
}
