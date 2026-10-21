/*
 * A055950  a(n) = n + reversal of base 5 digits of n (written in base 10).
 * https://oeis.org/A055950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = n + reverse_base5(n), where reverse_base5(n) is the
 *            number obtained by reversing the base‑5 digits of n (interpreted
 *            again in base 10)
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t rev = 0;
    idx_t m = n;

    /* generate the digits of n in base 5 (least significant first)   */
    while (m > 0) {
        seq_t digit = (seq_t)(m % 5);
        rev = rev * 5 + digit;   /* append digit to the reversed number */
        m = m / 5;
    }
    /* n is given as idx_t, cast to seq_t for addition                      */
    return (seq_t)n + rev;
}
