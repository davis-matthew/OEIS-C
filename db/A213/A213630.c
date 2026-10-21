/*
 * A213630  a(t_1 t_2...t_n) = (t_1 + t_2)*t_1 t_2 + ... + (t_n-1 + t_n)*t_n-1
 *          t_n.
 * https://oeis.org/A213630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = sum_{pairs (d1,d2)} (d1 + d2) * (10*d1 + d2), where pairs
 *            are the decimal digits of n taken in consecutive twos from left
 *            to right, prefixed by a leading 0 if the total number of digits
 *            is odd
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t temp = n;                  /* work on a copy */
    for (;;) {
        seq_t d2 = temp % 10;        /* least‑significant digit of the pair */
        temp /= 10;
        seq_t d1 = temp % 10;        /* next digit, 0 if none left */
        temp /= 10;
        seq_t pair = (d1 + d2) * (10 * d1 + d2);
        result += pair;
        if (temp == 0) {             /* all digits processed */
            break;
        }
    }
    return result;
}
