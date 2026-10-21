/*
 * A056524  Palindromes with even number of digits.
 * https://oeis.org/A056524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n*10^digits(n) + reverse(n) (concatenation of n and its
 *            reverse)
 * Range:     u8 n <= 9, u16 n <= 99, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"          /* provides seq_t pow10(idx_t e) */

seq_t a(idx_t n)
{
    /* compute number of decimal digits of n */
    idx_t m = n;
    idx_t digits = 0;
    do {
        digits++;
        m /= 10;
    } while (m != 0);

    /* compute reverse of n */
    seq_t rev = 0;
    m = n;
    while (m != 0) {
        rev = rev * 10 + (m % 10);
        m /= 10;
    }

    /* combine using pow10 */
    return (seq_t)n * pow10(digits) + rev;
}
